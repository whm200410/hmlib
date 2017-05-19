#include "MsgHander.h"
#include <algorithm>
#include <vector>
#include "MsgBus.h"
#include "Msg.h"
using namespace ns_hm_bus;
using namespace std;
map<CMsgHander*, CMsgHander::vMsgCmds_t>   CMsgHander::s_msgHandMaps;
map<CMsgHander*, CMsgHander::vMsgCmds_t>   CMsgHander::s_msgBlocked;

std::mutex                  CMsgHander::s_mutex;
CMsgHander::CMsgHander()
{
}


CMsgHander::~CMsgHander()
{
    std::lock_guard<std::mutex> guard(s_mutex);
    if (s_msgHandMaps.find(this) != s_msgHandMaps.end())
        s_msgHandMaps.erase(s_msgHandMaps.find(this));
}
void CMsgHander::registerMsgHander(int nCmd)
{
    std::lock_guard<std::mutex> guard(s_mutex);
    vMsgCmds_t  &vcmds = s_msgHandMaps[this];
    if (vcmds.find(nCmd) == vcmds.end())
        vcmds.insert(nCmd);

    msgHanderRegister(nCmd, msgHandler);
}
void CMsgHander::unregisterMsgHander(int nCmd)
{
    std::lock_guard<std::mutex> guard(s_mutex);
    vMsgCmds_t  &vcmds = s_msgHandMaps[this];
    if (vcmds.find(nCmd) != vcmds.end())
    {
        vcmds.erase(nCmd);
        if(vcmds.empty()) s_msgHandMaps.erase(this);
        if(s_msgHandMaps.empty()) msgHanderUnRegister(nCmd, msgHandler);
    }
}
void CMsgHander::blockMsg(int nCmd, bool bBlock)
{
    std::lock_guard<std::mutex> guard(s_mutex);
    vMsgCmds_t &vCmdsBlock = s_msgBlocked[this];
    if (bBlock && vCmdsBlock.find(nCmd) == vCmdsBlock.end())
        vCmdsBlock.insert(nCmd);
    else if (!bBlock && vCmdsBlock.find(nCmd) != vCmdsBlock.end())
        vCmdsBlock.erase(nCmd);
}
void CMsgHander::msgHandler(CMsg* pmsg)
{
    std::unique_lock<std::mutex> guard(s_mutex);

    int                     nCmd = pmsg->getCmd();
    vector<CMsgHander*>    vHands;

    typedef map<CMsgHander*, CMsgHander::vMsgCmds_t>  mpHanders_t;
    for (mpHanders_t::value_type &v : s_msgHandMaps)
    {
        vMsgCmds_t &vCmds = v.second;
        if (vCmds.find(nCmd) != vCmds.end())
        {
            vMsgCmds_t &vCmdsBlock = s_msgBlocked[v.first];
            if (vCmdsBlock.find(nCmd) == vCmdsBlock.end())
                vHands.push_back(v.first);
        }
    }
    guard.unlock();

    for (CMsgHander* v : vHands)
    {
        v->processMsg(pmsg);
    }
}
