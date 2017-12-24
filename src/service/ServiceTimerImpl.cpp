#include "ServiceTimerImpl.h"
#include "QueueService.h"
#include <chrono>
#include <vector>
using namespace std;
tEvent_t::tEvent_t(int mSec, CQueueService *pSer, const CMsg &msg)
    :m_pSer(pSer), m_msg(msg), m_nCmd(msg.getCmd())
{
    std::chrono::high_resolution_clock::time_point td = std::chrono::high_resolution_clock::now();
    m_tm = td + std::chrono::milliseconds(mSec);
}
bool operator <(const tEvent_t &lhs, const tEvent_t &rhs){
    return lhs.m_tm < rhs.m_tm;
}
void CServiceTimerImpl::insert(int msec, CQueueService* pSer, const CMsg &msg)
{
    std::lock_guard<std::mutex> gd(m_mutex);
    if(m_bExit) return;
    ptrEvent_t pevt(new tEvent_t(msec, pSer, msg));
    m_evs.insert(pevt);
    m_cond.notify_one();
}
void CServiceTimerImpl::erase(CQueueService* pSer, const CMsg *pMsg)
{
    std::lock_guard<std::mutex> gd(m_mutex);
    const bool                  bCanelAll = pMsg == 0;
    const int                   nCmd = pMsg == 0 ? 0 : pMsg->getCmd();
    vector<ptrEvent_t>          vEra;

    //find out all needs to be remove
    for(auto &v : m_evs)
    {
        if(v->m_pSer == pSer && (bCanelAll || v->m_nCmd == nCmd)) vEra.push_back(v);
    }

    //remove all
    for(auto &v : vEra)
    {
        m_evs.erase(v);
    }

    m_cond.notify_one();
}

CServiceTimerImpl::CServiceTimerImpl()
{
    m_bExit = false;
    m_tid = std::thread(&CServiceTimerImpl::threadFunc, this);
}

CServiceTimerImpl::~CServiceTimerImpl()
{
    stop();
}

void CServiceTimerImpl::stop()
{
    std::unique_lock<std::mutex>          gd(m_mutex);
    if (!m_bExit)
    {
        m_bExit = true;
        m_evs.clear();
        gd.unlock();
        m_cond.notify_all();
        m_tid.join();

    }
}

/*will be called before stop thread*/


bool    CServiceTimerImpl::bContinue()
{
    std::lock_guard<std::mutex> gd(m_mutex);
    return !m_bExit;
}
void    CServiceTimerImpl::doNext()
{
    std::unique_lock<std::mutex>          gd(m_mutex);

    auto tNow = std::chrono::high_resolution_clock::now();
    while (m_evs.empty() || tNow < (*m_evs.begin())->m_tm)
    {
        if (m_evs.empty())
        {
            m_cond.wait(gd);
        }
        else
        {
            m_cond.wait_for(gd, ( (*m_evs.begin())->m_tm - tNow));
        }
        if (m_bExit) return;
        tNow = std::chrono::high_resolution_clock::now();
    }
    if (m_bExit) return;

    ptrEvent_t pEvt = *m_evs.begin();
    pEvt->m_pSer->postMsg(pEvt->m_msg);
    m_evs.erase(pEvt);
}
/*actual job process function*/

void    CServiceTimerImpl::threadFunc(void)
{
    while (1)
    {
        if (m_bExit) break;
        doNext();
    }
}

