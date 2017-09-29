#pragma once

#include <QSize>
#include <QList>
#include "Common.h"
#include <memory>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace Magick
{
class Image;
}

namespace GSNImageToolBox
{
class ImageInfo
{
public:
    explicit ImageInfo(const QJsonDocument& jsonMetadata);
    ImageInfo(const ImageInfo& other);
    ImageInfo();
    ~ImageInfo();

    void print(quint8 imageNumber = 0) const;

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
    QJsonArray m_jsonMetadata;
    QSize m_thumbSize;
    QSize m_imageSize;
    quint8 m_bitsPerPixel;
    common::EColorSpace m_colorSpace;
    common::EImageFormat m_format;
};

} // namespace GSNImageToolBox

