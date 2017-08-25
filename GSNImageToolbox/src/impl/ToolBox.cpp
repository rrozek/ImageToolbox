#include "ToolBox.h"

#include "ImageMagick/Magick++.h"
#include "ImageMagick/MagickCore/magick-type.h"
#include <QDir>
namespace GSNImageToolBox
{

ToolBox::ToolBox()
{
    Magick::InitializeMagick(QDir::currentPath().toStdString().c_str());
}

ToolBox::~ToolBox()
{

}

void ToolBox::setSource(const char *data, size_t size)
{
    m_sourceImg.reset(new Magick::Blob(data, size));
}

const char* ToolBox::getImage(Common::EImageFormat format) const
{
    Magick::Image img(*(m_sourceImg.get()));
    img.alpha(static_cast<const unsigned int>(0));
    img.clip();
    img.alpha(static_cast<const unsigned int>(img.depth()));
    img.magick("png");
    img.write(m_outputImg.get());
    return static_cast<const char*>(m_outputImg->data());
}

bool ToolBox::collectImageInfo()
{
    return true;
}

const ImageInfo& ToolBox::getImageInfo() const
{
    return m_imageInfo;
}

} // namespace GSNImageToolBox
