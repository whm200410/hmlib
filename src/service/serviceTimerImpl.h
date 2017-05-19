#ifndef _SERVICE_TIMER_IMPL_H
#define _SERVICE_TIMER_H
#include "msg.h"
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

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
bool operator <(const tEvent_t &lhs, const tEvent_t &rhs);
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
    typedef std::multiset<tEvent_t> timeEvents_t;
    std::multiset<tEvent_t> m_evs;
    std::mutex              m_mutex;
    hmcondition             m_cond;
    std::thread             m_tid;
    bool                    m_bExit;
};
#endif
