#include "ImageInfo.h"

namespace GSNImageToolBox
{

ImageInfo::ImageInfo(QObject *parent) : QObject(parent)
{

}

bool ImageInfo::isContainer() const
{
    return false;
}

quint8 ImageInfo::getImagesCount() const
{
    return 1;
}

const ImageInfo &ImageInfo::getImageInfo(quint8 imageNumber) const
{
    return m_images.at(imageNumber);
}

bool ImageInfo::hasThumbnail() const
{
    return false;
}

} // namespace GSNImageToolBox
