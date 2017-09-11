#include "ToolBox.h"

#include "ImageMagick/Magick++.h"
#include <QDir>
#include <QDebug>

namespace GSNImageToolBox
{

ToolBox::ToolBox()
    : m_sourceImg(new Magick::Image())
{
    Magick::InitializeMagick(QDir::currentPath().toStdString().c_str());
}

ToolBox::~ToolBox()
{

}

void ToolBox::setSource(const char *data, size_t size)
{
    m_sourceBlob.reset(new Magick::Blob(data, size));

    try
    {
        QList<Magick::Image> images;
        Magick::readImages(&images,*(m_sourceBlob.get()));
        qDebug() << "########## dealing with multi-image data ##########";
        m_imageInfo.reset(new ImageInfo(images));
    }
    catch( Magick::Exception &/*error*/)
    {
        try
        {
            m_sourceImg->read(*(m_sourceBlob.get()));
            m_imageInfo.reset(new ImageInfo(*m_sourceImg));
        }
        catch( Magick::Exception &error)
        {
            qWarning() << "ImageMagick exception caught. Cannot read image.";
            qWarning() << error.what();
            return;
        }
    }
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
    try
    {
        m_outputBlob.reset(new Magick::Blob());
        Magick::Image img(*(m_sourceBlob.get()));

        applyMaskFromClippingPath(img, format);

        img.write(m_outputBlob.get());

        dataSize = m_outputBlob->length();

        char* returnArray = new char[dataSize];
        memcpy(returnArray, static_cast<const char*>(m_outputBlob->data()), m_outputBlob->length());

        return returnArray;
    }
    catch( Magick::Exception &error)
    {
        qDebug() << "caught ImageMagick exception. abc";
        qDebug() << error.what();
        dataSize = 0;
        return nullptr;
    }
}

void ToolBox::printImageInfo()
{
    m_imageInfo->print();
}

const ImageInfo& ToolBox::getImageInfo() const
{
    return *(m_imageInfo.get());
}

bool ToolBox::applyMaskFromClippingPath(Magick::Image &manipulatedImg, common::EImageFormat format)
{
    try
    {
        qDebug() << "manipulatedImg.magick()" << QString::fromStdString(manipulatedImg.magick());
        if (QString::fromStdString(manipulatedImg.magick()).contains("ps", Qt::CaseInsensitive)) // ps is for postscript
        {
            qDebug() << "applying 300dpi density";
            qDebug() << manipulatedImg.resolutionUnits();
            qDebug() << manipulatedImg.density().x() << manipulatedImg.density().y();
            Magick::Image tmpImg;
            tmpImg.resolutionUnits(Magick::PixelsPerInchResolution);
            tmpImg.density(Magick::Point(300,300));
            tmpImg.read(*m_sourceBlob.get());
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
