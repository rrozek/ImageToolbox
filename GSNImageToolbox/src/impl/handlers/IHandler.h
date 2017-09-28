#pragma once

#include "Common.h"
#include "ImageInfo.h"

#include <QJsonDocument>

namespace Magick
{
class Blob;
class Image;
}

namespace GSNImageToolBox
{
namespace handlers
{

class IHandler
{
public:
    IHandler(Magick::Blob &blob, const QJsonDocument& jsonImageInfo);
    virtual ~IHandler();

    void init();

    virtual quint8 getImageCount() const = 0;
    virtual char* getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize) = 0;

    virtual void printImageInfo() const;
    virtual const ImageInfo& getImageInfo() const;

protected:

    virtual void applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format) = 0;
    virtual void handleSource() = 0;

    std::shared_ptr<Magick::Blob> m_sourceBlob;
    std::unique_ptr<ImageInfo> m_imageInfo;
    QJsonDocument m_jsonImageInfo;
};

}
}
