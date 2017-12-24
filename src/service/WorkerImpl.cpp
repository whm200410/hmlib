#include "Worker.h"
#include "WorkerImpl.h"
#include "QueueService.h"
#include <algorithm>
#include "Msg.h"
using namespace std;

CWorkerImpl::task_t::task_t()
{
    setState(eIdle);
}

void
CWorkerImpl::task_t::setState(taskState e)
{
    std::unique_lock<std::mutex> lockGrab(mutex);
    state = e;
    lockGrab.unlock();
    cond.notify_one();
}

CWorkerImpl::taskState
CWorkerImpl::task_t::getState()
{
    std::unique_lock<std::mutex> lockGrab(mutex);
    return state;
}

void        CWorkerImpl::clearWorks()
{
    std::unique_lock<std::mutex> lockGrab(m_mutex);
    m_tasks.clear();
}

CWorkerImpl::prtTask_t CWorkerImpl::findTask(task_t *pTask)
{
    std::list<prtTask_t>::iterator it = std::find_if(m_tasks.begin(), m_tasks.end(), [&](const prtTask_t &tk)->bool{return tk.get() == pTask;});
    if(it == m_tasks.end()) return prtTask_t();
    return *it;
}

CWorkerImpl::task_t*
CWorkerImpl::postWork(CWorker::workHander_t hd, const CMsg &msg)
{
    if(!isStarted())
    {
        startService();
    }

    std::unique_lock<std::mutex> lockGrab(m_mutex);
    prtTask_t pTask = std::make_shared<task_t>();
    pTask->hander   = hd;
    pTask->msg      = msg;
    m_tasks.push_back(pTask);
    postMsg(CMsg(eCmdProcessTask, (void*)pTask.get()));
    return pTask.get();
}

bool        CWorkerImpl::isDone(task_t *pTask)
{
    prtTask_t   task;
    {
        std::unique_lock<std::mutex> lockGrab(m_mutex);
        task = findTask(pTask);
        if(task == nullptr) return true;
    }

    std::unique_lock<std::mutex> lockGrab(task->mutex);
    return task->getState() == eDone;

}

void        CWorkerImpl::waitWork(task_t *pTask)
{
    prtTask_t   task;
    {
        std::unique_lock<std::mutex> lockGrab(m_mutex);
        task = findTask(pTask);
        if(task == nullptr) return;
    }

    std::unique_lock<std::mutex> lockTaskGrab(task->mutex);
    task->cond.wait(lockTaskGrab);
}

void        CWorkerImpl::waitAllWorks()
{
    waitJobs();
}

void CWorkerImpl::launch(CMsg* msg)
{
    if(msg->getCmd() == eCmdProcessTask)
    {
        task_t *pTask = (task_t*)msg->getPoint(0);
        prtTask_t   task;

        {
            std::unique_lock<std::mutex> lockGrab(m_mutex);
            task = findTask(pTask);
        }
        if(task)
        {
            task->setState(eWorking);
            task->hander(&pTask->msg);
            task->setState(eDone);
            std::unique_lock<std::mutex> lockGrab(m_mutex);
            m_tasks.remove(task);
        }
    }
}
