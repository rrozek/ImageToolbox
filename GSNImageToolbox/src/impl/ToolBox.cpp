#include "ToolBox.h"

#include "ImageMagick/Magick++.h"
#include <QDir>
#include <QDebug>

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

char* ToolBox::getImage(Common::EImageFormat format, size_t &dataSize)
{
    m_outputImg.reset(new Magick::Blob());
    Magick::Image img(*(m_sourceImg.get()));

    applyMaskFromClippingPath(img, format);

    img.write(m_outputImg.get());

    dataSize = m_outputImg->length();
    char* data = new char[dataSize];
    memcpy(data, static_cast<const char*>(m_outputImg->data()), dataSize);

    return data;
}

QByteArray ToolBox::getImage(Common::EImageFormat format)
{
    m_outputImg.reset(new Magick::Blob());
    Magick::Image img(*(m_sourceImg.get()));

    applyMaskFromClippingPath(img, format);

    img.write(m_outputImg.get());

    QByteArray dataArray(static_cast<const char*>(m_outputImg->data()), m_outputImg->length());
    return dataArray;
}

bool ToolBox::collectImageInfo()
{
    return true;
}

const ImageInfo& ToolBox::getImageInfo() const
{
    return m_imageInfo;
}

bool ToolBox::applyMaskFromClippingPath(Magick::Image &manipulatedImg, Common::EImageFormat format)
{
    try
    {
        manipulatedImg.alphaChannel(MagickCore::TransparentAlphaChannel);
        manipulatedImg.clip();
        manipulatedImg.alphaChannel(MagickCore::OpaqueAlphaChannel);

        manipulatedImg.magick(Common::EImageFormatString[format]);
    }
    catch( Magick::Exception &error)
    {
        qDebug() << "could not apply mask from clipping path." << error.what();
        return false;
    }
    return true;

}

} // namespace GSNImageToolBox
