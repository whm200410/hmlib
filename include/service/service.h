#ifndef _HMService_H
#define _HMService_H
#include <memory>
#include "hmlib_global.h"

//*****************************************************************************
// Name       : service.h
// Description: service concept which is implemeted by thread,
//              service provide basic operation for a independant task
//              you need to inherit and overrite doRun and doStop,
//              doStop is what derived class need to do to quit run,
//              it may contain setting a quit flag or others.
// Author     : Wang Hui Ming
// Date       : 2016-08-10
// Copyright ©: 2001-2016 by Siemens AG, PTD EM
// History    :
// ----------------------------------------------------------------------------
//*****************************************************************************

class  CServiceImpl;
class  HMLIBSHARED_EXPORT CService
{
public:
    friend class CServiceImpl;

    //start, stop and wait thread
    bool start();
    void stop();
    void wait();

protected:
    CService();
    virtual ~CService();

    //actual job process function
    virtual void doRun() = 0;

    //overrite this to provide your specific stop action to quit run
    virtual void doStop() = 0;
private:
    std::unique_ptr<CServiceImpl> m_impl;
};
#endif
