#pragma once

#include <QString>
#include "gsnimagetoolbox_global.h"

namespace GSNImageToolBox
{
namespace common
{

enum EColorSpace
{ RGB
, ARGB
, RGBA
, BGR
};

enum EImageFormat
{ TIFF = 0
, PNG
, EPS
, JPEG

, COUNT
};

static const char* EImageFormatString[] =
{ "tiff"
, "png"
, "eps"
, "jpeg"
};

static const char* EImageFormatStringCapital[] =
{ "Tiff"
, "Png"
, "Eps"
, "Jpeg"
};

GSNIMAGETOOLBOXSHARED_EXPORT EImageFormat EImageFormatFromString(QString format);

} // namespace Common
} // namespace GSNImageToolBox
