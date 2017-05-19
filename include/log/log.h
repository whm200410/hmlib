#ifndef LOG_H
#define LOG_H
#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_QT_LOGGING
#define ELPP_FORCE_USE_STD_THREAD
#include "easylogging++.h"
#include "hmlib_global.h"

#define LogInfo()  LOG(INFO)
#define LogError() LOG(ERROR)
#define LogWarn()  LOG(WARNING)

namespace HLog
{
   HMLIBSHARED_EXPORT void LogInit(const char *szConfig);
}

#endif // LOG_H
