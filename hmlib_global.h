#ifndef HMLIB_GLOBAL_H
#define HMLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HMLIB_LIBRARY)
#  define HMLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HMLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HMLIB_GLOBAL_H
