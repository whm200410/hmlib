#include <map>
#include <vector>
#include "Msg.h"
using std::map;
class CMsgImgl
{
public:
	void    setRet(int idx, int nRet){ m_mpRet[idx] = nRet; }
	int	    getRet(int idx){ return m_mpRet[idx]; }

    void    setInt(int idx, int v){ m_iparams[idx] = v; }
    int     getInt(int idx){ return m_iparams[idx]; }

    void    setBool(int idx, bool v){ m_bparams[idx] = v; }
    bool    getBool(int idx){ return m_bparams[idx]; }

    void    setDoub(int idx, double v){ m_dparams[idx] = v; }
    double  getDoub(int idx){ return m_dparams[idx]; }

    void    setPoint(int idx, void* v){ m_pparams[idx] = v; }
    void*   getPoint(int idx){ return m_pparams[idx]; }

    string  getString(int idx){ return m_szparams[idx]; }
    void    setString(int idx, const string &str){ m_szparams[idx] = str; }
private:
    void clear();
private:
    map<int, string>    m_szparams;
    map<int, int>       m_iparams;
    map<int, void*>     m_pparams;
    map<int, double>    m_dparams;
    map<int, bool>      m_bparams;
	map<int, int>       m_mpRet;

};

CMsg::CMsg(int ncmd)
{
    m_ncmd          = ncmd;
}
CMsg::CMsg(int ncmd, void* param)
{
    m_ncmd  = ncmd;
    setPoint(0, param);
}
CMsg::CMsg(int ncmd, const string& sparam)
{
    m_ncmd  = ncmd;
    setString(0, sparam);
}

CMsg::CMsg(int ncmd, int nparam)
{
    m_ncmd  = ncmd;
    setInt(0, nparam);
}

CMsg::~CMsg()
{

}

void CMsg::clone(CMsg *msg)
{
    checkValidMsg();
    m_ncmd = msg->m_ncmd;
    *pmsg = *msg->pmsg;
}

bool CMsg::isEventMsg()
{
    checkValidMsg();
    return pmsg->getBool(cEvtMarkIdx);
}

void CMsg::markEventMsg()
{
    checkValidMsg();
    pmsg->setBool(cEvtMarkIdx, true);
}

int CMsg::getEvtId()
{
    checkValidMsg();
    return pmsg->getInt(cEvtIdIdx);
}

void CMsg::setEvtId(int nId)
{
    checkValidMsg();
    pmsg->setInt(cEvtIdIdx, nId);
}

void    CMsg::setInt(int idx, int v)
{
    checkValidMsg();
    pmsg->setInt(idx, v);
}

int     CMsg::getInt(int idx)
{
    checkValidMsg();
    return pmsg->getInt(idx);
}
void    CMsg::setBool(int idx, bool v)
{
    checkValidMsg();
    pmsg->setBool(idx, v);
}
bool    CMsg::getBool(int idx)
{
    checkValidMsg();
    return pmsg->getBool(idx);
}
void    CMsg::setDoub(int idx, double v)
{
    checkValidMsg();
    pmsg->setDoub(idx, v);
}
double  CMsg::getDoub(int idx)
{
    checkValidMsg();
    return pmsg->getDoub(idx);
}
void    CMsg::setPoint(int idx, void* v)
{
    checkValidMsg();
    pmsg->setPoint(idx, v);
}
void*   CMsg::getPoint(int idx)
{
    checkValidMsg();
    return pmsg->getPoint(idx);
}
string  CMsg::getString(int idx)
{
    checkValidMsg();
    return pmsg->getString(idx);
}
void    CMsg::setString(int idx, const string &str)
{
    checkValidMsg();
    pmsg->setString(idx, str);
}
void    CMsg::setCmd(int cmd_)
{
    m_ncmd = cmd_;
}
int     CMsg::getCmd() const
{
    return m_ncmd;
}

void    CMsg::setRet(int idx, int nRet)
{
    checkValidMsg();
    pmsg->setRet(idx, nRet);
}
int	    CMsg::getRet(int idx)
{
    checkValidMsg();
    return pmsg->getRet(idx);
}
void    CMsg::checkValidMsg()
{
    if (!pmsg) pmsg = std::make_shared<CMsgImgl>();
}
