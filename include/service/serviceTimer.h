//*****************************************************************************
// Name       : serviceTimer.h
// Description: Time is used to insert in event or job to a CQueueService at some future time,
//              when that time meets, the task will be excuate in CQueueService's over loop
// Author     : Wang Hui Ming
// Date       : 2016-08-10
// Copyright ©: 2001-2016 by Siemens AG, PTD EM
// History    :
// ----------------------------------------------------------------------------
//*****************************************************************************
#ifndef _SERVICE_TIMER_H
#define _SERVICE_TIMER_H
#include <memory>
#include "hmlib_global.h"
class CQueueService;
class CServiceTimerImpl;
class CMsg;

class HMLIBSHARED_EXPORT CServiceTimer
{
public:
    CServiceTimer();
    virtual ~CServiceTimer();
    void stop();
    void insert(int msec, CQueueService*, const CMsg &msg);
    void erase(CQueueService*, const CMsg *pMsg = 0);
private:   
    std::unique_ptr<CServiceTimerImpl> m_impl;
};
#endif
