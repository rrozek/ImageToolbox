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
    IHandler(std::shared_ptr<Magick::Blob> blob, const QJsonDocument& jsonImageInfo);
    virtual ~IHandler();

    void init();

    virtual quint8 getImageCount() const = 0;
    virtual char* getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize) = 0;

    virtual void printImageInfo() const;
    virtual const ImageInfo& getImageInfo() const;


    virtual char *getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, size_t &dataSize) = 0;
    virtual char *getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, size_t &dataSize);
    virtual char *getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, size_t &dataSize) = 0;
    virtual char *getThumbnail(float thumbPercentSize, quint8 imageNumber, size_t &dataSize);
protected:

    virtual void applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format) = 0;
    virtual void handleSource() = 0;

    std::shared_ptr<Magick::Blob> m_sourceBlob;
    std::unique_ptr<ImageInfo> m_imageInfo;
    QJsonDocument m_jsonImageInfo;
};

}
}
