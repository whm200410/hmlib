//*****************************************************************************
// Name       : msg.h
// Description: basic structure used to broadcast event and post job
// Author     : Wang Hui Ming
// Date       : 2016-08-10
// Copyright  : 2001-2016 by Siemens AG, PTD EM
// History    :
// ----------------------------------------------------------------------------
//*****************************************************************************


#ifndef _HMMSG_H
#define _HMMSG_H
#include <string>
#include <memory>
#include "hmlib_global.h"
using std::string;



class CMsgImgl;
class HMLIBSHARED_EXPORT CMsg
{
public:
	CMsg(int ncmd = cUnknowCmd);
	CMsg(int ncmd, void* param);
	CMsg(int ncmd, int nparam);
	CMsg(int ncmd, const string& sparam);
	~CMsg();

    void    clone(CMsg *msg);
    bool    isEventMsg();
    void    markEventMsg();

    int     getEvtId();
    void    setEvtId(int nId);

	void    setInt(int idx, int v);
	int     getInt(int idx);

	void    setBool(int idx, bool v);
	bool    getBool(int idx);

	void    setDoub(int idx, double v);
	double  getDoub(int idx);

        
	void    setPoint(int idx, void* v);
	void*   getPoint(int idx);

	string  getString(int idx);
	void    setString(int idx, const string &str);

	void    setCmd(int cmd_);
	int     getCmd() const;

    void    setRet(int idx, int nRet);
    int	    getRet(int idx);
    static const int cUnknowCmd     = -1;
    static const int cExitCmd       = -1234;
    static const int cEvtMarkIdx    = 0x981234;
    static const int cEvtIdIdx      = 0x981235;
private:
	void    checkValidMsg();
    typedef std::shared_ptr<CMsgImgl>    msgImplPtr_t;
private:
	int             m_ncmd;
	msgImplPtr_t    pmsg;
};

#endif

