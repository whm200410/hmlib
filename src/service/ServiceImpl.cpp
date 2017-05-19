#include "Service.h"
#include "ServiceImpl.h"


CServiceImpl::CServiceImpl(CService *pParent)
    :m_parent(pParent)
{
    setState(stStoped);
}

CServiceImpl::~CServiceImpl()
{
    stop();
    wait();
}

bool CServiceImpl::start()
{
    if(getState() == stWorking) return true;
    m_thread = std::thread(&CServiceImpl::runThread, this);
    while(getState() != stWorking);
    return true;
}

void CServiceImpl::stop()
{
    if(getState() == stStoped)return;
    m_parent->doStop();

}
void CServiceImpl::wait()
{
    if(getState() == stStoped) return;
    m_thread.join();

}

void  CServiceImpl::runThread()
{
    setState(stWorking);
    m_parent->doRun();
    setState(stStoped);
}

void
CServiceImpl::setState(EServiceState e)
{
    std::unique_lock<std::mutex> lockGrab(m_mutexState);
    m_state = e;
}

CServiceImpl::EServiceState
CServiceImpl::getState()
{
    std::unique_lock<std::mutex> lockGrab(m_mutexState);
    return m_state;
}
