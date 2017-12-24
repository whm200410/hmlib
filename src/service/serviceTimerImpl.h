#ifndef _SERVICE_TIMER_IMPL_H
#define _SERVICE_TIMER_H
#include "msg.h"
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <memory>
class CQueueService;
class CMsg;
struct tEvent_t
{
    tEvent_t(int mSec, CQueueService *pSer, const CMsg &msg);
    typedef std::chrono::high_resolution_clock::time_point absTime_t;
    absTime_t       m_tm;
    CQueueService   *m_pSer;
    CMsg            m_msg;
    int             m_nCmd;
};

typedef std::shared_ptr<tEvent_t> ptrEvent_t;
struct evtCompare {
    bool operator()(const ptrEvent_t &evt1, const ptrEvent_t &evt2) const {
        return evt1->m_tm < evt2->m_tm;
    }
};




bool operator<(const ptrEvent_t &evt1, const ptrEvent_t &evt2);
class CServiceTimerImpl
{
public:
    CServiceTimerImpl();
    virtual ~CServiceTimerImpl();

    void stop();
    void insert(int msec, CQueueService*, const CMsg &msg);
    void erase(CQueueService*, const CMsg *pMsg = 0);
protected:
    void    threadFunc(void);
    bool    bContinue();
    void    doNext();
private:

    typedef std::condition_variable hmcondition;
    std::multiset<ptrEvent_t, evtCompare> m_evs;
    std::mutex              m_mutex;
    hmcondition             m_cond;
    std::thread             m_tid;
    bool                    m_bExit;
};
#endif
