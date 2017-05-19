#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "hmlib_global.h"
namespace hm_utils {

//get current excuable path
HMLIBSHARED_EXPORT std::string getExecutablePath(void);

//get current excuable folder
HMLIBSHARED_EXPORT std::string getExecutableDirectory(void);

//get some file inside excuable path
HMLIBSHARED_EXPORT std::string getExecutableSpecFile(const std::string &);


//get file name from a path
HMLIBSHARED_EXPORT std::string getFileName(const std::string &sPath);


template<class T>
size_t str2Array(const std::string &str, std::vector<T> &v)
{
    v.clear();
    std::stringstream 		isstream(str);
    std::istream_iterator<T> iter(isstream);
    std::istream_iterator<T> iter_end;
    std::copy(iter, iter_end, std::back_inserter(v));
    return v.size();
}

template<class out_type, class in_value>
out_type convert(const in_value & t)
{
    std::stringstream stream;

    stream << t;

    out_type result;

    stream >> result;

    return result;
}
template<class T>
void str2Array(const char*str, const char* delim, std::vector<T> &v)
{
    v.clear();
    char *conte;
    char *token = strtok_s((char *)str, delim, &conte);
    while (token != NULL)
    {
        T val = convert<T>(token);
        v.push_back(val);

        token = strtok_s(NULL, delim, &conte);
    }
}
HMLIBSHARED_EXPORT std::string wstring2string(const std::wstring &str);
HMLIBSHARED_EXPORT std::wstring string2wstring(const std::string &str);
}



#endif // UTILS_H
