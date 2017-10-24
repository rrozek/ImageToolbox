#pragma once

#include <QDebug>
#include <QString>
#include "gsnimagetoolbox_global.h"

namespace GSNImageToolBox
{

namespace common
{

class GSNIMAGETOOLBOXSHARED_EXPORT Utils
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

