#pragma once

#include "gsnimagetoolbox_global.h"

//#include "ImageMagick/Magick++/Image.h"

#include "ImageInfo.h"

#include <memory>

namespace GSNImageToolBox
{
namespace handlers
{
class IHandler;
}
}

namespace Magick
{
class Blob;
class Image;
}

namespace GSNImageToolBox
{

class GSNIMAGETOOLBOXSHARED_EXPORT ToolBox
{

public:
    ToolBox();
    ~ToolBox();

    void setSource(const char* data, size_t size);

    void getImage(common::EImageFormat format, QByteArray &dataArray);
    void getImage(quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray);

    char* getImage(common::EImageFormat format, size_t &dataSize);
    char* getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize);

    quint8 getImageCount() const;

    void printImageInfo();
    const ImageInfo& getImageInfo() const;

private:
    bool applyMaskFromClippingPath(Magick::Image& manipulatedImg, common::EImageFormat format);

    std::unique_ptr<handlers::IHandler> m_handler;
};

} // namespace GSNImageToolBox
