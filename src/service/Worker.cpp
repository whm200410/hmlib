#include "Worker.h"
#include "WorkerImpl.h"
#include "QueueService.h"
#include "Msg.h"
using namespace std;

CWorker::workId_t::workId_t(const workImpl_t &impl, void *param)
{
    m_impl  = impl;
    m_param = param;
}

bool CWorker::workId_t::isDone()
{
    workImpl_t ptr = m_impl.lock();
    if(!ptr) return true;
    return ptr->isDone( (CWorkerImpl::task_t *)m_param);
}

void CWorker::workId_t::wait()
{
    workImpl_t ptr = m_impl.lock();
    if(!ptr) return;
    ptr->waitWork((CWorkerImpl::task_t *)m_param);
}

CWorker::CWorker()
{
    m_impl = std::make_shared<CWorkerImpl>();
}

CWorker::~CWorker()
{
    stop();
}

void        CWorker::start()
{
     m_impl->startService();
}

void        CWorker::stop()
{
    clearWorks();
    m_impl->stopService();
}


void        CWorker::waitAllWorks()
{
    m_impl->waitAllWorks();
}
void        CWorker::clearWorks()
{
    m_impl->clearWorks();
}

CWorker::workId_t
CWorker::doPostWork(workHander_t wk, const CMsg &msg)
{
    workId_t id(m_impl,  m_impl->postWork(wk, msg));
    return id;
}
