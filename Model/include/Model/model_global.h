#pragma once

#include <QtGlobal>

// -----------------------------------------------------------------------------
// Symbol export/import macros for the Model library
// -----------------------------------------------------------------------------
//
// Usage:
//   - In your .pro file, define MODEL_LIBRARY when building the library itself:
//         DEFINES += MODEL_LIBRARY
//
//   - In your headers, prefix exported classes/functions with MODEL_EXPORT:
//         class MODEL_EXPORT Transaction : public QObject { ... };
//
// -----------------------------------------------------------------------------

#if defined(MODEL_LIBRARY)
#  define MODEL_EXPORT Q_DECL_EXPORT
#else
#  define MODEL_EXPORT Q_DECL_IMPORT
#endif
