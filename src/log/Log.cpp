#include "Log.h"
#include <string>
#include "HMLogHelper.h"
#include "utils.h"
#include "HMSystemCfgMgr.h"
namespace HLog
{

    void LogInit(std::string sCfg, std::string sName, std::string szFolder)
    {
        const std::string sFmt = "%datetime{%Y_%M_%d-%H_%m_%m}";
        std::string sname = fmt::format("{}\\{}.{}.log", szFolder, sName, sFmt);
//        std::string sname = HMSystemCfgMgr::getProgramMainSubFolder("logs/XRay.%datetime{%Y_%M_%d-%H_%m_%m}.log");
        el::Configurations conf(sCfg);
        conf.set(el::Level::Global, el::ConfigurationType::Filename, sname);
        // Reconfigure single logger
        el::Loggers::reconfigureLogger("default", conf);
        // Actually reconfigure all loggers instead
        el::Loggers::reconfigureAllLoggers(conf);

        installCrashHandler(szFolder);
    }
}
