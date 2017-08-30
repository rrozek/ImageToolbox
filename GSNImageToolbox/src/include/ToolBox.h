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

    void getImage(common::EImageFormat format, QByteArray &dataArray);
    char* getImage(common::EImageFormat format, size_t &dataSize);

    bool collectImageInfo();
    const ImageInfo& getImageInfo() const;

private:
    bool applyMaskFromClippingPath(Magick::Image& manipulatedImg, common::EImageFormat format);

    std::unique_ptr<Magick::Blob> m_sourceImg;
    std::unique_ptr<Magick::Blob> m_outputImg;
    ImageInfo m_imageInfo;
};

} // namespace GSNImageToolBox
