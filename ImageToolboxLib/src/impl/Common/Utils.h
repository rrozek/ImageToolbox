#pragma once

#include <QDebug>
#include <QString>
#include "ImageToolboxLib_global.h"

namespace GSNImageToolBox
{

namespace common
{

class IMAGETOOLBOXLIB_SHARED_EXPORT Utils
{
public:
    Utils();

    static QString filterImagesRegex;
    static QStringList filterImagesList;

private:
    static QStringList initList();
};

} // namespace common
} // namespace GSNImageToolBox

