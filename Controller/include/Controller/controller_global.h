#pragma once

#include <QtGlobal>

#if defined(CONTROLLER_LIBRARY)
#  define CONTROLLER_EXPORT Q_DECL_EXPORT
#else
#  define CONTROLLER_EXPORT Q_DECL_IMPORT
#endif
