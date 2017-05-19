#include "ServiceTimerImpl.h"
#include "QueueService.h"
#include <chrono>

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
    tEvent_t tev(msec, pSer, msg);
    m_evs.insert(tev);
    m_cond.notify_one();
}
void CServiceTimerImpl::erase(CQueueService* pSer, const CMsg *pMsg)
{
    std::lock_guard<std::mutex> gd(m_mutex);
    const bool                  bCanelAll = pMsg == 0;
    const int                   nCmd = pMsg == 0 ? 0 : pMsg->getCmd();

    std::list<const tEvent_t*> evts;
    typedef std::multiset<tEvent_t>::iterator eIt_t;

    //we can replace the remove by lambda or remove_if
    for (eIt_t it = m_evs.begin(); it != m_evs.end(); ++it)
    {
        const tEvent_t &ev = *it;
        if (it->m_pSer == pSer && (bCanelAll || it->m_nCmd == nCmd))
        {
            evts.push_back(&ev);
        }
    }

    size_t nPreSz = m_evs.size();
    while (!evts.empty())
    {
        const tEvent_t *pEv = evts.front();
        for (eIt_t it = m_evs.begin(); it != m_evs.end(); ++it)
        {
            const tEvent_t &ev = *it;
            if (&ev == pEv)
            {
                m_evs.erase(it);
                break;
            }
        }
    }
    size_t nNowSz = m_evs.size();

    if (nNowSz != nPreSz) m_cond.notify_one();
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
    while (m_evs.empty() || tNow < m_evs.begin()->m_tm)
    {
        if (m_evs.empty())
        {
            m_cond.wait(gd);
        }
        else
        {
            m_cond.wait_for(gd, (m_evs.begin()->m_tm - tNow));
        }
        if (m_bExit) return;
        tNow = std::chrono::high_resolution_clock::now();
    }
    if (m_bExit) return;

    std::multiset<tEvent_t>::iterator itFirst = m_evs.begin();
    itFirst->m_pSer->postMsg(itFirst->m_msg);
    m_evs.erase(itFirst);  
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
