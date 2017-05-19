#ifndef MSG_SIGNAL_H
#define MSG_SIGNAL_H
#include "hmlib_global.h"
//#include "msgSignalDefs.h"
#include "nod.hpp"
#include "msgSignalDef.h"
#include <map>
#include "msg.h"
#include <memory>
#include <mutex>
#include <functional>
#include <vector>
#include <initializer_list>
class HMLIBSHARED_EXPORT msgSignal
{
public:
    msgSignal();
    virtual ~msgSignal();
    typedef ns_signal::connection_t                 connection_t;
    typedef nod::signal<void (CMsg*)>               HMSignal_t;
    typedef nod::signal<void (CMsg*)>::slot_type    callSig_t;

    void copySignals(msgSignal*);

    template<typename Callable>
    connection_t subscribeEvent(int nCmd, Callable call)
    {
        return doSubscribeEvent(nCmd, call);
    }

    template<class F, class A>
    connection_t subscribeEvent(int nCmd, F f, A a)
    {
        return doSubscribeEvent(nCmd, std::bind(f,a, std::placeholders::_1));
    }

    template<class F, class A>
    std::vector<connection_t> subscribeEvent(std::initializer_list<int> vv, F f, A a)
    {
        std::vector<connection_t> vCon;
        for(auto &v : vv)
            vCon.push_back(doSubscribeEvent(v, std::bind(f,a, std::placeholders::_1)));
        return vCon;
    }

    void unSubscribe(int nCmd);
    void unSubscribe(int nCmd, connection_t&);

    void invokeEvent(int nCmd, CMsg* msg);
    void invokeEvent(CMsg &msg)
    {
        invokeEvent(msg.getCmd(), &msg);
    }

    void invokeEvent(int nCmd, CMsg &msg)
    {
        invokeEvent(nCmd, &msg);
    }

protected:
    typedef std::shared_ptr<HMSignal_t> ptrSignal_t;
    std::map<int, ptrSignal_t>          m_mpSigs;
    std::mutex                          m_mutex;
private:
    connection_t doSubscribeEvent(int, const callSig_t &subscriber);
};



#endif // MSGSIGAL_H
