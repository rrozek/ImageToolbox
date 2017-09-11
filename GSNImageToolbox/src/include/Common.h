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

static_assert(sizeof(GSNImageToolBox::common::EImageFormatString) / sizeof(char*) == GSNImageToolBox::common::COUNT
              , "EImageFormat and EImageFormatString don't match. Did you forget to specify string representation of new enum?");

static EImageFormat EImageFormatFromString(QString format)
{
    EImageFormat returnVal = COUNT;
    for ( quint8 i = 0; i < common::COUNT; ++i )
    {
        if (QString(EImageFormatString[i]).contains(format, Qt::CaseInsensitive))
        {
            returnVal = static_cast<common::EImageFormat>(i);
            break;
        }
    }
    return returnVal;
}

} // namespace Common
} // namespace GSNImageToolBox
