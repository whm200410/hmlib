#include "Log.h"
#include <string>


namespace HLog
{

    void LogInit(const char *sCfg)
    {

        el::Configurations conf(sCfg);
        // Reconfigure single logger
        el::Loggers::reconfigureLogger("default", conf);
        // Actually reconfigure all loggers instead
        el::Loggers::reconfigureAllLoggers(conf);
    }
}



