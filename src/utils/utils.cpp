#include "utils.h"
#include <locale>
#include <codecvt>
using std::string;
#define BOOST_OS_WINDOWS 1

#if (BOOST_OS_WINDOWS)
#  include <stdlib.h>
#elif (BOOST_OS_SOLARIS)
#  include <stdlib.h>
#  include <limits.h>
#elif (BOOST_OS_LINUX)
#  include <unistd.h>
#  include <limits.h>
#elif (BOOST_OS_MACOS)
#  include <mach-o/dyld.h>
#elif (BOOST_OS_BSD_FREE)
#  include <sys/types.h>
#  include <sys/sysctl.h>
#endif
namespace hm_utils {

//get current excuable path
std::string getExecutablePath(void)
{
#if (BOOST_OS_WINDOWS)
    char *exePath;
    if (_get_pgmptr(&exePath) != 0)
        return "";
#elif (BOOST_OS_SOLARIS)
    char exePath[PATH_MAX];
    if (realpath(getexecname(), exePath) == NULL)
        exePath[0] = '\0';
#elif (BOOST_OS_LINUX)
    char exePath[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", exePath, sizeof(exePath));
    if (len == -1 || len == sizeof(exePath))
        len = 0;
    exePath[len] = '\0';
#elif (BOOST_OS_MACOS)
    char exePath[PATH_MAX];
    uint32_t len = sizeof(exePath);
    if (_NSGetExecutablePath(exePath, &len) != 0) {
        exePath[0] = '\0'; // buffer too small (!)
    } else {
        // resolve symlinks, ., .. if possible
        char *canonicalPath = realpath(exePath, NULL);
        if (canonicalPath != NULL) {
            strncpy(exePath,canonicalPath,len);
            free(canonicalPath);
        }
    }
#elif (BOOST_OS_BSD_FREE)
    char exePath[2048];
    int mib[4];  mib[0] = CTL_KERN;  mib[1] = KERN_PROC;  mib[2] = KERN_PROC_PATHNAME;  mib[3] = -1;
    size_t len = sizeof(exePath);
    if (sysctl(mib, 4, exePath, &len, NULL, 0) != 0)
        exePath[0] = '\0';
#endif
    return std::string(exePath);
}

//get current excuable folder
std::string getExecutableDirectory(void)
{
    std::string sExc = getExecutablePath();
    sExc = sExc.substr(0, sExc.find_last_of('\\'));
 //   sExc.find_last_of()

 //   path        sExc(getExecutablePath());
 //   return sExc.remove_filename().string();
    return sExc;
}

//get some file inside excuable path
std::string getExecutableSpecFile(const std::string &sName)
{
    string sPath = getExecutableDirectory();
    return sPath + "\\" + sName;
}

std::string wstring2string(const std::wstring &str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(str);
}

std::wstring string2wstring(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string getFileName(const std::string &sPath)
{
#ifdef _WIN32
    char cD = '\\';
#else
    char cD = '/';
#endif

    auto nPos = sPath.find_last_of(cD);
    if(nPos == std::string::npos) return sPath;
    else return sPath.substr(nPos + 1);
}

}
