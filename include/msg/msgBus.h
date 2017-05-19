//*****************************************************************************
// Name       : msgBus.h
// Description: event broadcast / subscribe system, it's thread safe
// Author     : Wang Hui Ming
// Date       : 2016-08-10
// Copyright  : 2001-2016 by Siemens AG, PTD EM
// History    :
// ----------------------------------------------------------------------------
//*****************************************************************************


#ifndef _HMMSG_BUS_H
#define _HMMSG_BUS_H
#include "Msg.h"
#include "hmlib_global.h"
namespace ns_hm_bus
{
    typedef void(*hmMsgHander_t)(CMsg*);

    /*Register and unregister msg handler*/
    HMLIBSHARED_EXPORT void msgHanderRegister(int nCmd, hmMsgHander_t);

    HMLIBSHARED_EXPORT void msgHanderUnRegister(int nCmd, hmMsgHander_t);

    /*spreed cmds, the module who register the cmd will hander it*/
    HMLIBSHARED_EXPORT void msgInvokeCmd(int nCmd, CMsg &msg);

    HMLIBSHARED_EXPORT void msgBlock(int nCmd, bool bBlock);
}
#endif
