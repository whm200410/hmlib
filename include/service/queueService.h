#ifndef _HMQueueService_H
#define _HMQueueService_H
#include <string>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "hmlib_global.h"
using std::string;
using std::list;

typedef std::condition_variable hmcondition;

class CMsg;
class  HMLIBSHARED_EXPORT CQueueService
{
public:
    /*after post msg, thread will work on this, if service is grabbed by other threads, 
    need to wait until other thread ungrab*/
    friend class HMThreadMgr;
    void postMsg(const CMsg&, bool bAvoidDup = false);

    /*start thread, will monitor jobs and call launch of derived class */
    bool startService();
    void stopService();

    /*if several thread need to ask service exclusively, need to use grab, ungrab*/
    bool grab();
    bool unGrab();
    /*wait for all jobs complete*/
    void waitJobs();

    /*query*/
    bool isJobEmpty();
    bool isGrabed();
    bool isStarted();
    bool isBusy();
    virtual string  getMsgName(int nCmd);
    void emergencyStop();
protected:
    CQueueService(const string &sname);
    virtual ~CQueueService();

    /*virtual functions derived class the to inherit to start thread,
    init will do some device init,*/
    virtual bool init() = 0;

    /*will be called after thread started up*/
    virtual void startSpecific() =0;

    /*will be called before stop thread*/
    virtual bool stopPreprare() = 0;
    virtual void clearResource() = 0;

    /*actual job process function*/
    virtual void launch(CMsg*) = 0;

    string  getServiceName();


protected:
    enum HMServiceState_e
    {
        stCreated, stWaitJob, stWorking, stStoped
    };
    void                setState(HMServiceState_e);
    HMServiceState_e    getState();


private:
    static void thread_func(CQueueService*);
    int         do_thread_func();
    void        doStopService();
private:
    HMServiceState_e    m_state;
    list<CMsg>         m_jobs;

    std::mutex          m_mutexJobs;
    std::mutex          m_mutexState;
    std::mutex          m_mutexGrab;
    std::mutex          m_mutexJobEmpty;

    hmcondition         m_cdtJobs;
    hmcondition         m_cdtWaitGrab;
    hmcondition         m_cdtWaitJobEmpty;

    std::thread         m_thread;
    std::thread::id     m_grabId;
    bool                m_grabed;
    string              m_name;
};

class HMLIBSHARED_EXPORT CQueueServiceSimple : public CQueueService
{
public:
    CQueueServiceSimple(const string &sname)
        :CQueueService(sname){}
protected:
    virtual bool init() {return true;}

    /*will be called after thread started up*/
    virtual void startSpecific(){}

    /*will be called before stop thread*/
    virtual bool stopPreprare() {return true;}
    virtual void clearResource(){}
};
#endif
