#include "msgSignalDefs2.h"


void HMSignal_t::operator()(CMsg* pmsg)
{
    m_sig(pmsg);
}

HMSignal_t::connection_t HMSignal_t::connect(const HMSignal_t::callSig_t &func)
{
    return m_sig.connect(func);
}

void HMSignal_t::disconnect(HMSignal_t::connection_t *con)
{
    con->disconnect();
}

void HMSignal_t::disconnect_all_slots()
{
    m_sig.disconnect_all_slots();
}
