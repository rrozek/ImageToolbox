#pragma once

#include "gsnimagetoolbox_global.h"

#include "ImageInfo.h"
#include <memory>

namespace Magick
{
class Blob;
}

namespace GSNImageToolBox
{

class GSNIMAGETOOLBOXSHARED_EXPORT ToolBox
{

public:
    ToolBox();
    ~ToolBox();

    void setSource(const char* data, size_t size);

    const char *getImage(Common::EImageFormat format) const;

    bool collectImageInfo();
    const ImageInfo& getImageInfo() const;

private:
    std::unique_ptr<Magick::Blob> m_sourceImg;
    std::unique_ptr<Magick::Blob> m_outputImg;
    ImageInfo m_imageInfo;
};

} // namespace GSNImageToolBox
