#include "UtilsGUI.h"

namespace GSNImageToolBox
{

namespace common
{

UtilsGUI::UtilsGUI()
{
    placeHolderIcons = initIcons();
}

const QIcon UtilsGUI::getIcon(EImageFormat format) const
{
    return placeHolderIcons.value(format);
}

QMap<EImageFormat, QIcon> UtilsGUI::initIcons()
{
    QMap<EImageFormat, QIcon> iconMap;
    EImageFormat formatCounter = TIFF;
    while(formatCounter < EImageFormat::COUNT)
    {
        iconMap[formatCounter] = fromResource(QString(":/thumbs/resources/") + QString(EImageFormatString[formatCounter]) + QString(".png"));
        // yup, huge hack
        quint8 formatCounterIncrementator = formatCounter;
        formatCounterIncrementator++;
        formatCounter = static_cast<EImageFormat>(formatCounterIncrementator);
    }
    return iconMap;
}

QIcon UtilsGUI::fromResource(QString path)
{
    QIcon icon(QPixmap::fromImage(QImage(path, "PNG")));
    return icon;
}

} // namespace common
} // namespace GSNImageToolBox

