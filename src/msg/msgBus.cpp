#include "MsgBus.h"
#include <set>
#include <map>
#include <algorithm>
#include <memory>
#include <mutex>

using namespace std;


namespace ns_hm_bus
{

    typedef set<hmMsgHander_t>          vFuncs_t;
    typedef map<int, vFuncs_t>          mpHanders_t;
    typedef map<int, bool>              mpBlock_t;

    struct msgBusInfo_t
    {
        mpHanders_t         m_mpMsgHanders;
        mpBlock_t           m_blockInfo;
        std::mutex          m_mutex;
    };

    msgBusInfo_t &busInfo(){
        static msgBusInfo_t s_info;
  //      if(s_info == 0) s_info = new msgBusInfo_t();
        return s_info;
    }
    
    /*Register and unregister msg handler*/
    void msgHanderRegister(int ncmd, hmMsgHander_t hander)
    {
        std::lock_guard<std::mutex> guard(busInfo().m_mutex);
        vFuncs_t &funs = busInfo().m_mpMsgHanders[ncmd];
        if (find(funs.begin(), funs.end(), hander) == funs.end())
            funs.insert(hander);
    }

    void msgHanderUnRegister(int ncmd, hmMsgHander_t hander)
    {
        std::lock_guard<std::mutex> guard(busInfo().m_mutex);
        vFuncs_t &funs = busInfo().m_mpMsgHanders[ncmd];
        funs.erase(hander);

    }
    void msgBlock(int ncmd, bool bBlock)
    {
        std::lock_guard<std::mutex> guard(busInfo().m_mutex);
        mpBlock_t& iBlock = busInfo().m_blockInfo;
        iBlock[ncmd] = bBlock;
    }
    /*spreed cmds, the module who register the cmd will hander it*/

    void msgInvokeCmd(int ncmd, CMsg &msg)
    {
        std::unique_lock<std::mutex> guard(busInfo().m_mutex);
        mpBlock_t& iBlock = busInfo().m_blockInfo;
        if(iBlock[ncmd]) return;

        msg.setCmd(ncmd);
        vFuncs_t funs = busInfo().m_mpMsgHanders[ncmd];
        guard.unlock();
        for(hmMsgHander_t v : funs) (v)(&msg);
    }
}
