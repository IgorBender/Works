#ifndef PTHREADCLASSLIB_GLOBAL_H
#define PTHREADCLASSLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PTHREADCLASSLIB_LIBRARY)
#  define PTHREADCLASSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PTHREADCLASSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PTHREADCLASSLIB_GLOBAL_H
