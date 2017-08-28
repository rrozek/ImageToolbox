#pragma once

#include "gsnimagetoolbox_global.h"

//#include "ImageMagick/Magick++/Image.h"

#include "ImageInfo.h"
#include <memory>

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

    char* getImage(Common::EImageFormat format, size_t &dataSize);
    QByteArray getImage(Common::EImageFormat format);

    bool collectImageInfo();
    const ImageInfo& getImageInfo() const;

private:
    bool applyMaskFromClippingPath(Magick::Image& manipulatedImg, Common::EImageFormat format);

    std::unique_ptr<Magick::Blob> m_sourceImg;
    std::unique_ptr<Magick::Blob> m_outputImg;
    ImageInfo m_imageInfo;
};

} // namespace GSNImageToolBox
