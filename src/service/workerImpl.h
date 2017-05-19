#ifndef WORKER_IMPL_H
#define WORKER_IMPL_H
#include "queueService.h"
#include "msg.h"
#include <memory>
#include <mutex>
class CWorkerImpl : public CQueueServiceSimple
{
public:
    CWorkerImpl()
        :CQueueServiceSimple("worker"){}

    //need to clear all jobs and stop service
    ~CWorkerImpl()
    {
        clearWorks();
        stopService();
    }

    //the command this service need to hander
    enum workCmd{
        eCmdProcessTask = 100,
    };

    //each task's state
    enum taskState{
        eIdle,
        eWorking,
        eDone,
    };

    //task information, mechanism to wait and access
    struct task_t
    {
        task_t();
        void        setState(taskState e);
        taskState   getState();

        CWorker::workHander_t       hander;
        CMsg                        msg;
        std::mutex                  mutex;
        std::condition_variable     cond;
        taskState                   state;
    };
    typedef std::shared_ptr<task_t> prtTask_t;


    task_t*     postWork(CWorker::workHander_t, const CMsg &msg);

    //check or wait task to finish
    bool        isDone(task_t *pTask);
    void        waitWork(task_t *pTask);

    //wait all tasks
    void        waitAllWorks();


    void        clearWorks();
protected:
    void launch(CMsg*) ;
private:
    prtTask_t findTask(task_t *pTask);
private:
    std::list<prtTask_t> m_tasks;
    std::mutex          m_mutex;
};

#endif // WORKER_H
