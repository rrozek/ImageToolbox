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

void ToolBox::getImage(common::EImageFormat format, QByteArray& dataArray)
{
    size_t dataSize = 0;
    char* rawArray = getImage(format, dataSize);
    dataArray = QByteArray(rawArray, dataSize);
    delete[] rawArray;
}

char* ToolBox::getImage(common::EImageFormat format, size_t& dataSize)
{
    m_outputImg.reset(new Magick::Blob());
    Magick::Image img(*(m_sourceImg.get()));

    applyMaskFromClippingPath(img, format);

    img.write(m_outputImg.get());

    dataSize = m_outputImg->length();

    char* returnArray = new char[dataSize];
    memcpy(returnArray, static_cast<const char*>(m_outputImg->data()), m_outputImg->length());

    return returnArray;
}

bool ToolBox::collectImageInfo()
{
    return true;
}

const ImageInfo& ToolBox::getImageInfo() const
{
    return m_imageInfo;
}

bool ToolBox::applyMaskFromClippingPath(Magick::Image &manipulatedImg, common::EImageFormat format)
{
    try
    {
        qDebug() << "manipulatedImg.magick()" << QString::fromStdString(manipulatedImg.magick());
        if (QString::fromStdString(manipulatedImg.magick()).contains("ps", Qt::CaseInsensitive)) // ps is for postscript
        {
            qDebug() << "applying 300dpi density";
            Magick::Image tmpImg;
            tmpImg.resolutionUnits(Magick::PixelsPerInchResolution);
            tmpImg.density(Magick::Point(300,300));
            tmpImg.read(*m_sourceImg.get());
            manipulatedImg = tmpImg;
        }
        manipulatedImg.alphaChannel(MagickCore::TransparentAlphaChannel);
        manipulatedImg.clip();
        manipulatedImg.alphaChannel(MagickCore::OpaqueAlphaChannel);

        manipulatedImg.magick(common::EImageFormatString[format]);
    }
    catch( Magick::Exception &error)
    {
        qDebug() << "could not apply mask from clipping path." << error.what();
        return false;
    }
    return true;

}

} // namespace GSNImageToolBox
