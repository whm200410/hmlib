#include "msgSignal.h"

void msgSignal::unSubscribe(int nCmd)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if(m_mpSigs.find(nCmd) == m_mpSigs.end()) return;
    m_mpSigs[nCmd]->disconnect_all_slots();
    m_mpSigs.erase(nCmd);
}

msgSignal::connection_t
msgSignal::doSubscribeEvent(int nCmd, const callSig_t &subscriber)
{
    std::lock_guard<std::mutex> guard(m_mutex);

    if(m_mpSigs.find(nCmd) == m_mpSigs.end())
    {
        m_mpSigs[nCmd].reset(new HMSignal_t());
    }
    return m_mpSigs[nCmd]->connect(subscriber);
}
msgSignal::msgSignal()
{

}
void msgSignal::copySignals(msgSignal* pSig)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_mpSigs.clear();
    m_mpSigs = pSig->m_mpSigs;
}

msgSignal::~msgSignal()
{
    for(auto &mp : m_mpSigs)
    {
        mp.second->disconnect_all_slots();
    }
    m_mpSigs.clear();
}
void msgSignal::invokeEvent(int nCmd, CMsg* msg)
{
    std::unique_lock<std::mutex> guard(m_mutex);
    msg->markEventMsg();
    msg->setCmd(nCmd);
    if(m_mpSigs.find(nCmd) == m_mpSigs.end()) return;
    ptrSignal_t &pSig   = m_mpSigs[nCmd];
    guard.unlock();
    (*pSig.get())(msg);
}
void msgSignal::unSubscribe(int , connection_t& con)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    con.disconnect();

}
