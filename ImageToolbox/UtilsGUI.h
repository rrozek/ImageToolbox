#pragma once

#include <QDebug>
#include <QString>
#include <QMap>
#include <QIcon>

#include "Common.h"

namespace GSNImageToolBox
{

namespace common
{

const QString dirConversionDestination = "Dir/ConversionDestination"; // not in use currently
const QString dirBaseDir = "Dir/BaseDirectory";
class UtilsGUI
{
public:
    UtilsGUI();

    const QIcon getIcon(EImageFormat format) const;
private:
    QMap<EImageFormat,QIcon> placeHolderIcons;
    QMap<EImageFormat,QIcon> initIcons();
    QIcon fromResource(QString path);

};

} // namespace common
} // namespace GSNImageToolBox

