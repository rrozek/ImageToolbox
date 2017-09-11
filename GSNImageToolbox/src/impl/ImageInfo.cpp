#include "ImageInfo.h"

#include <QDebug>
#include "ImageMagick/Magick++.h"

namespace GSNImageToolBox
{

ImageInfo::ImageInfo(const QList<Magick::Image> &images, QObject *parent)
    : QObject(parent)
{
    collectImageInfo(images);
}

ImageInfo::ImageInfo(const Magick::Image& image, QObject *parent)
    : QObject(parent)
{
    collectImageInfo(image);
}

ImageInfo::ImageInfo(const ImageInfo &other)
{
    for ( ImageInfo info : other.m_images)
    {
        m_images.append(info);
    }

    m_thumbSize = other.m_thumbSize;
    m_imageSize = other.m_imageSize;
    m_bitsPerPixel = other.m_bitsPerPixel;
    m_colorSpace = other.m_colorSpace;
    m_format = other.m_format;
}

void ImageInfo::print() const
{
    if (isContainer())
    {
        for ( const ImageInfo& info : m_images )
            info.print();
    }
    else
    {
        qDebug() << "Image size: " << m_imageSize;
        qDebug() << "Image format: " << m_format << "as string: " << common::EImageFormatString[m_format];
        qDebug() << "Bits per pixel: " << m_bitsPerPixel;
        qDebug() << "Colorspace: Comin soon.";
    }
}

bool ImageInfo::isContainer() const
{
    if (m_images.size() > 1)
        return true;
    return false;
}

quint8 ImageInfo::getImagesCount() const
{
    return m_images.size();
}

const ImageInfo &ImageInfo::getImageInfo(quint8 imageNumber) const
{
    return m_images.at(imageNumber);
}

bool ImageInfo::hasThumbnail() const
{
    return false;
}

void ImageInfo::collectImageInfo(const QList<Magick::Image> &images)
{
    for ( const Magick::Image& image : images)
    {
        ImageInfo info(image);
        m_images.append(info);
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
