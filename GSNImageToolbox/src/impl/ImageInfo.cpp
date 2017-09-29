#include "ImageInfo.h"

#include <QDebug>
#include "Magick++.h"

namespace GSNImageToolBox
{

ImageInfo::ImageInfo(const QJsonDocument &jsonMetadata)
{
    if (jsonMetadata.isArray())
        m_jsonMetadata = jsonMetadata.array();
    else if (jsonMetadata.isObject())
        m_jsonMetadata.append(jsonMetadata.object());
    else if (jsonMetadata.isEmpty() || jsonMetadata.isNull())
        qWarning() << "Invalid metadata";
}

ImageInfo::ImageInfo(const ImageInfo &other)
{
    m_jsonMetadata = other.m_jsonMetadata;
    m_thumbSize = other.m_thumbSize;
    m_imageSize = other.m_imageSize;
    m_bitsPerPixel = other.m_bitsPerPixel;
    m_colorSpace = other.m_colorSpace;
    m_format = other.m_format;
}

ImageInfo::ImageInfo()
{

}

ImageInfo::~ImageInfo()
{
    m_images.clear();
}

void ImageInfo::print(quint8 imageNumber) const
{
    if ( m_jsonMetadata.size() <= imageNumber )
    {
        qWarning() << "requested print info of image=" << imageNumber << "but only " << m_jsonMetadata.size() << "is avaliable";
        return;
    }
    qDebug() << m_jsonMetadata[imageNumber];
}

bool ImageInfo::isContainer() const
{
    if (m_jsonMetadata.size() > 1)
        return true;
    return false;
}

quint8 ImageInfo::getImagesCount() const
{
    return m_jsonMetadata.size();
}

const ImageInfo &ImageInfo::getImageInfo(quint8 imageNumber) const
{
    if ( m_jsonMetadata.size() <= imageNumber )
    {
        qWarning() << "requested info of image=" << imageNumber << "but only " << m_jsonMetadata.size() << "is avaliable";
        return ImageInfo::invalid;
    }
    return
}

bool ImageInfo::hasThumbnail() const
{
    return false;
}

void ImageInfo::collectImageInfo(const QList<Magick::Image> &images)
{
    for ( const Magick::Image& image : images)
    {
        m_images.append(std::shared_ptr<ImageInfo>(new ImageInfo(image)));
    }
}

void ImageInfo::collectImageInfo(const Magick::Image &image)
{
    m_imageSize.setWidth(image.baseColumns());
    m_imageSize.setHeight(image.baseRows());

    m_format = common::EImageFormatFromString(QString::fromStdString(image.magick()));

    m_bitsPerPixel = image.depth();
    // TODO: COLORSPACE
}

} // namespace GSNImageToolBox
