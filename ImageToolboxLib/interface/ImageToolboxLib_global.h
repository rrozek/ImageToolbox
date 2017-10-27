#pragma once

#include <QtCore/qglobal.h>

#if defined(IMAGETOOLBOXLIB)
#  define IMAGETOOLBOXLIB_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGETOOLBOXLIB_SHARED_EXPORT Q_DECL_IMPORT
#endif

