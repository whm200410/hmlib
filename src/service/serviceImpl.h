#ifndef _Service_IMPL_H
#define _Service_IMPL_H
#include <thread>
#include <mutex>


class  CServiceImpl
{
public:
    CServiceImpl(CService *pParent);
    ~CServiceImpl();
    /*start thread*/
    bool start();
    void stop();
    void wait();
protected:
    enum EServiceState
    {
        stStoped,
        stWorking
    };
    void                setState(EServiceState);
    EServiceState       getState();
    void                runThread();
private:
    EServiceState       m_state;
    std::mutex          m_mutexState;
    std::thread         m_thread;
    CService            *m_parent;
};
#endif
