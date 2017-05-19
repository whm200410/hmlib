//*****************************************************************************
// Name       : msg.h
// Description: basic structure used to broadcast event and post job
//              *this view is used to inherit from to implement event handler.
//              the theory is that when GUI recives an event/msg, it needs
//              to update its content, this action should be done at the
//              GUI event thread, or there should be some issues.
//              ex: event is broadcasted in another thread, so
//              virtual void processMsg(CMsg*) = 0; will be called from
//              that thread, if view what to do some GUI action, in the
//              virtual void processMsg(CMsg*) = 0;, it can call postMsg or
//              forceMsg to post/send msg to GUI event loop what will handle
//              that event/msg in processMsgInWndLoop, function
//              processMsgInWndLoop is always called in GUI thread, so
//              it's safe to call any GUI actions here.
//
//              the difference of postMsg and forceMsg is same as
//              QCoreApplication::postEvent and QCoreApplication::sendEvent

// Author     : Wang Hui Ming
// Date       : 2016-08-10
// Copyright ©: 2001-2016 by Siemens AG, PTD EM
// History    :
// ----------------------------------------------------------------------------
//*****************************************************************************

#ifndef MSGHANDERVIEW_H
#define MSGHANDERVIEW_H
#include <list>
#include <QWidget>
#include "Msg.h"
#include <memory>
#include <mutex>
#include "MsgHander.h"

template<class tView>
class CMsgHanderView : public tView, public CMsgHander
{
public:
    CMsgHanderView(QWidget *pParent = 0)
        :tView(pParent){}
    virtual ~CMsgHanderView(){}
protected:
    /* derived class overwrite this function to do actions
     * in GUI thread
     */
    virtual void processMsgInGuiThread(CMsg*) = 0;

    /* this two function is usually used in void processMsg(CMsg*) = 0 which
     * which is defined at CMsgHander, forceMsg will wait GUI process to finish,
     * postMsg just post that task to GUI.
    */
    void    postMsg(CMsg*, bool bAvoidDup = false);
    void    forceMsg(CMsg*, bool bAvoidDup = false);
private:
    bool    getFrontMsgAndPop(CMsg &msg);
    bool    pushBackMsg(CMsg *, bool bAvoidDup = false);
    void    triggerMsgPrcess();
    void    customEvent(QEvent *event);
private:
    std::list<CMsg>    m_lMsgs;
    std::mutex        m_mutex;
    static const int C_CUS_EVT_ID   = 1024;
};

#include "msgHanderView.hpp"

#endif // MSGHANDERVIEW_H
