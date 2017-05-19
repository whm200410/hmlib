//*****************************************************************************
// Name       : worker.h
// Description: worker is such a object that it can push task into queue to excute in a separete thread,
//              the queue is FIFO style, you can wait for each task that is pushed in
// Author     : Wang Hui Ming
// Date       : 2016-08-10
// Copyright  : 2001-2016 by Siemens AG, PTD EM
// History    :
// ----------------------------------------------------------------------------
//*****************************************************************************

#ifndef WORKER_H
#define WORKER_H


#include <functional>
#include <memory>
#include "hmlib_global.h"
class CMsg;
class CWorkerImpl;

class HMLIBSHARED_EXPORT CWorker
{
public:
    CWorker();
    ~CWorker();

    typedef std::weak_ptr<CWorkerImpl>    workWeakImpl_t;
    typedef std::shared_ptr<CWorkerImpl>  workImpl_t;
    typedef std::function<void (CMsg*)>   workHander_t;
    class workId_t
    {
    public:
        friend class CWorker;
        workId_t(){m_param = 0;}
        workId_t(const workImpl_t &impl, void *param);
        bool isDone();
        void wait();
    private:
        workWeakImpl_t  m_impl;
        void            *m_param;
    };
    //if start is not called in initialization,
    //it will be called when post the first task
    void        start();
    void        stop();

    template<typename Callable>
    workId_t    postWork(Callable call, const CMsg &msg)
    {
        doPostWork(call, msg);
    }

    template <class F,class A>
    workId_t    postWork(F f,A a, const CMsg &msg)
    {
        return doPostWork(std::bind(f,a, _1), msg);
    }

    void        clearWorks();
    void        waitAllWorks();
private:
    workId_t    doPostWork(workHander_t work, const CMsg &msg);
private:
    workImpl_t  m_impl;
};

#endif // WORKER_H
