#pragma once

#include <QString>

namespace GSNImageToolBox
{
namespace Common
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

static_assert(sizeof(GSNImageToolBox::Common::EImageFormatString) / sizeof(char*) == GSNImageToolBox::Common::COUNT
              , "EImageFormat and EImageFormatString don't match. Did you forget to specify string representation of new enum?");

} // namespace Common
} // namespace GSNImageToolBox
