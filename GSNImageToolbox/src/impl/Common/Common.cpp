#include "Common.h"

namespace GSNImageToolBox
{
namespace common
{

static_assert(sizeof(EImageFormatString) / sizeof(char*) == COUNT
              , "EImageFormat and EImageFormatString don't match. Did you forget to specify string representation of new enum?");


EImageFormat EImageFormatFromString(QString format)
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
