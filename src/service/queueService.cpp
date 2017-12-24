#include <map>
#include <chrono>
#include "queueService.h"
#include "Msg.h"
#include <sstream>



using namespace std;

#if 0
#include <windows.h>
#include <ProductKey.h>
#ifdef WIN32
#pragma comment(lib, "User32.lib")
#endif
namespace
{
    bool s_bLicensed = false;
}
#endif
void CQueueService::postMsg(const CMsg& msg, bool bAvoidDup)
{
    std::unique_lock<std::mutex> lockGrab(m_mutexGrab);
    while (m_grabed && m_grabId != std::this_thread::get_id())
    {
        m_cdtWaitGrab.wait(lockGrab);
    }
    std::unique_lock<std::mutex> lockJob(m_mutexJobs);

    if(bAvoidDup)
        for(auto &v : m_jobs)
            if(v.getCmd() == msg.getCmd()) return;

    bool bempty = m_jobs.empty();

    m_jobs.push_back(msg);

    if (bempty)
    {
        m_cdtJobs.notify_one();
    }

}
bool CQueueService::startService()
{
    if (isStarted())
    {
        return true;
    }
    if (!init())
    {
        return false;
    }
    m_grabed = false;
    m_thread = std::thread(thread_func, this);
    startSpecific();
    while (!isStarted())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return true;
}
void CQueueService::thread_func(CQueueService* parm)
{
    CQueueService *pServ = (CQueueService*)parm;
    
    pServ->do_thread_func();

}
int CQueueService::do_thread_func()
{
    CMsg msg;
    do 
    {
        setState(stWaitJob);
        std::unique_lock<std::mutex> lockJob(m_mutexJobs);
        while (m_jobs.empty())
        {
            m_cdtWaitJobEmpty.notify_one();
            m_cdtJobs.wait(lockJob);
        }
        msg = m_jobs.front();
        m_jobs.pop_front();
        setState(stWorking);
        lockJob.unlock();
        launch(&msg);


    } while (msg.getCmd() != CMsg::cExitCmd);
    setState(stStoped);
    return msg.getCmd();
}
void CQueueService::doStopService()
{
    stopPreprare();
    postMsg(CMsg(CMsg::cExitCmd));
    m_thread.join();
    clearResource();
    setState(stStoped);
}
void CQueueService::stopService()
{
    if (!isStarted()) return;
    doStopService();
}
bool CQueueService::grab()
{
    if (!isStarted()) return false;

    std::unique_lock<std::mutex> lockGrab(m_mutexGrab);
    if (m_grabed && m_grabId == std::this_thread::get_id())
    {
        return true;
    }
    while (m_grabed)
    {
        m_cdtWaitGrab.wait(lockGrab);
    }
    if (m_grabed && m_grabId == std::this_thread::get_id()) return true;
    m_grabId    = std::this_thread::get_id();
    m_grabed    = true;

    return true;
}
bool        CQueueService::isJobEmpty()
{
    std::unique_lock<std::mutex> lockJob(m_mutexJobs);
    return m_jobs.empty();
}
void CQueueService::waitJobs()
{
    if (!isStarted()) return;
    std::unique_lock<std::mutex> lockJobEmpty(m_mutexJobEmpty);
    while (!isJobEmpty() || isBusy())
    {
        m_cdtWaitJobEmpty.wait(lockJobEmpty);
    }
}
bool CQueueService::unGrab()
{
    if (!isStarted()) return false;

    std::unique_lock<std::mutex> lockGrab(m_mutexGrab);
    if (!m_grabed)
    {
        return false;
    }
    if (m_grabId != std::this_thread::get_id())
    {
        return false;
    }
    m_grabed = false;
    m_cdtWaitGrab.notify_one();

    return true;
}
bool CQueueService::isBusy()
{
    return (getState() == stWorking);
}
bool CQueueService::isGrabed()
{
    std::unique_lock<std::mutex> lockGrab(m_mutexGrab);
    return m_grabed;
}
string  CQueueService::getServiceName()
{
    return m_name;
}
string  CQueueService::getMsgName(int nCmd)
{
    std::ostringstream str;
    str << nCmd;
    return str.str();
}
CQueueService::CQueueService(const string &sname)
    :m_name(sname)
{
    m_grabed        = false;
    setState(stCreated);
}
CQueueService::~CQueueService()
{
//    assert(!isStarted());
}

CQueueService::HMServiceState_e    CQueueService::getState()
{
    std::unique_lock<std::mutex> lockGrab(m_mutexState);
    return m_state;
}
void CQueueService::setState(HMServiceState_e e)
{
    std::unique_lock<std::mutex> lockGrab(m_mutexState);
    m_state = e;

}
bool CQueueService::isStarted()
{
    std::unique_lock<std::mutex> lockGrab(m_mutexState);
    return m_state == stWaitJob || m_state == stWorking;
}
void CQueueService::emergencyStop()
{
 //   m_thread.interrupt();
}
#if 0
bool HMLibRegister(string szLicense, string &strError)
{
    bool bRet = false;
    ProductKey prokey;
    if(!prokey.GetCustomerLicenseInfo(szLicense.c_str()))
    {
        strError = string("license file : ") + string(szLicense) + " not exists.";
        bRet = false;
    }
    else if(!prokey.isHdidMatch())
    {
        strError = string("Id in file : ") + string(szLicense) + " not match with local id";
        bRet = false;
    }
    else if(!prokey.isKeyValid())
    {
        if(prokey.isExpired())
        {
            strError = string("License file has expired, please update license");
            bRet = false;
        }
        else
        {
            char szBuf[100] = {0};
            sprintf_s(szBuf, 100, "HMLib has %d days remainning , please reminder youself", prokey.remainingDays());
            strError = szBuf;
            bRet = true;
        }
    }
    else
    {
        strError = "license check pass";
        bRet = true;
    }

    if(szLicense == "whm200410@163.com")
    {
        bRet = true;
        strError = "trick for huiming";
    }
    s_bLicensed = bRet;
    return s_bLicensed;
}

bool isHmLibLicensed()
{
    return s_bLicensed;
}
#endif
