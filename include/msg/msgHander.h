//*****************************************************************************
// Name       : msgBus.h
// Description: This class is used to process msg in c++ class model
//              derived class need to override function processMsg
// Author     : Wang Hui Ming
// Date       : 2016-08-10
// Copyright  : 2001-2016 by Siemens AG, PTD EM
// History    :
// ----------------------------------------------------------------------------
//*****************************************************************************


#ifndef _HMMSG_HANDLER_H
#define _HMMSG_HANDLER_H
#include <map>
#include <set>
#include <mutex>
#include "hmlib_global.h"

class   CMsg;
class HMLIBSHARED_EXPORT CMsgHander
{
public:
    CMsgHander();
    virtual ~CMsgHander();
    /*the registered msg will be handled in processMsg*/

    void registerMsgHander(int nCmd);
    void unregisterMsgHander(int nCmd);

    void blockMsg(int nCmd, bool bBlock);
protected:
    virtual void processMsg(CMsg*) = 0;
private:
    static void msgHandler(CMsg*);
private:
    typedef std::set<int> vMsgCmds_t;
    static std::map<CMsgHander*, vMsgCmds_t>   s_msgHandMaps;
    static std::map<CMsgHander*, vMsgCmds_t>   s_msgBlocked;
    static std::mutex                          s_mutex;
};

#endif
