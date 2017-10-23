#pragma once

#include <QString>

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

static EImageFormat EImageFormatFromString(QString format);

} // namespace Common
} // namespace GSNImageToolBox
