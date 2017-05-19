#include "ServiceTimer.h"
#include "QueueService.h"
#include "ServiceTimerImpl.h"
void CServiceTimer::insert(int msec, CQueueService* pSer, const CMsg &msg)
{
    m_impl->insert(msec, pSer, msg);
}
void CServiceTimer::erase(CQueueService* pSer, const CMsg *pMsg)
{
    m_impl->erase(pSer, pMsg);
}
CServiceTimer& CServiceTimer::inst()
{
    static CServiceTimer x;return x;
}
CServiceTimer::CServiceTimer()
{
    m_impl.reset(new CServiceTimerImpl());
}

CServiceTimer::~CServiceTimer()
{
    stop();
}

void CServiceTimer::stop()
{
    m_impl->stop();
}
