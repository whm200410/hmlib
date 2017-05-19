#include <QEvent>
#include <QCoreApplication>




template<class tView>
void    CMsgHanderView<tView>::postMsg(CMsg *pmsg, bool bAvoidDup)
{
    pushBackMsg(pmsg, bAvoidDup);
    QEvent * pev = new QEvent((QEvent::Type)C_CUS_EVT_ID);
    QCoreApplication::postEvent(this, pev);
}

template<class tView>
void    CMsgHanderView<tView>::forceMsg(CMsg *pmsg, bool bAvoidDup)
{
    pushBackMsg(pmsg);
    QEvent * pev = new QEvent((QEvent::Type)C_CUS_EVT_ID);
    QCoreApplication::sendEvent(this, pev);
}

template<class tView>
void    CMsgHanderView<tView>::customEvent(QEvent *event)
{
    if(event->type() == C_CUS_EVT_ID)
    {
        triggerMsgPrcess();
    }
    else
    {
        QWidget::customEvent(event);
    }

}

template<class tView>
bool    CMsgHanderView<tView>::getFrontMsgAndPop(CMsg &msg)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if (m_lMsgs.empty()) return false;
    msg = m_lMsgs.front();
    m_lMsgs.pop_front();
    return true;
}
template<class tView>
bool    CMsgHanderView<tView>::pushBackMsg(CMsg *pmsg, bool bAvoidDup)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if(bAvoidDup)
    {
        for(auto &v : m_lMsgs)
        {
            if(v.getCmd() == pmsg->getCmd()) return false;
        }
    }
    m_lMsgs.push_back(*pmsg);
    return true;
}
template<class tView>
void    CMsgHanderView<tView>::triggerMsgPrcess()
{
    CMsg msg;
    while (getFrontMsgAndPop(msg))
    {
        processMsgInGuiThread(&msg);
    }
}
