#pragma once

#include <QSize>
#include <QList>
#include "Common.h"
#include <memory>

#include <QJsonObject>
#include <QJsonArray>

namespace Magick
{
class Image;
}

namespace GSNImageToolBox
{
class ImageInfo
{
public:
    explicit ImageInfo(const QList<Magick::Image>& images);
    ImageInfo(const Magick::Image& image);
    ImageInfo(const ImageInfo& other);
    ImageInfo();
    ~ImageInfo();

    void print() const;

    bool isContainer() const;
    quint8 getImagesCount() const;
    const ImageInfo& getImageInfo(quint8 imageNumber = 0) const;
    bool hasThumbnail() const;

    const QSize& getThumbSize() const { return m_thumbSize; }
    const QSize& getImageSize() const { return m_imageSize; }
    quint8 getBitsPerPixel() const { return m_bitsPerPixel; }
    common::EColorSpace getColorSpace() const { return m_colorSpace; }
    common::EImageFormat getImageFormat() const { return m_format; }

    static ImageInfo invalid;
private:

    void collectImageInfo(const QList<Magick::Image>& images);
    void collectImageInfo(const Magick::Image& image);

    QList<std::shared_ptr<ImageInfo>> m_images;
//    Exiv2::ExifData m_exif;
    QSize m_thumbSize;
    QSize m_imageSize;
    quint8 m_bitsPerPixel;
    common::EColorSpace m_colorSpace;
    common::EImageFormat m_format;
};

} // namespace GSNImageToolBox

