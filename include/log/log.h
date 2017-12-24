#ifndef LOG_H
#define LOG_H
#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_QT_LOGGING
#define ELPP_FORCE_USE_STD_THREAD
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING
#include "easylogging++.h"
#include "hmlib_global.h"
#include "format.h"
#define LogInfo()  LOG(INFO)
#define LogError() LOG(ERROR)
#define LogWarn()  LOG(WARNING)
namespace HLog
{
   void LogInit(std::string sCfg, std::string sName, std::string szFolder);
}

#endif // LOG_H
