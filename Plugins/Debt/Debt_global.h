#ifndef DEBT_GLOBAL_H
#define DEBT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DEBT_LIBRARY)
#  define DEBT_EXPORT Q_DECL_EXPORT
#else
#  define DEBT_EXPORT Q_DECL_IMPORT
#endif

#endif // DEBT_GLOBAL_H
