#ifndef BUDGET_GLOBAL_H
#define BUDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BUDGET_LIBRARY)
#  define BUDGET_EXPORT Q_DECL_EXPORT
#else
#  define BUDGET_EXPORT Q_DECL_IMPORT
#endif

#endif // BUDGET_GLOBAL_H
