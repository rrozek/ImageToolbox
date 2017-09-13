#include "SingleImageHandler.h"

#include "Magick++.h"

#include <QDebug>

namespace GSNImageToolBox
{
namespace handlers
{

SingleImageHandler::SingleImageHandler(Magick::Blob &blob)
    : IHandler(blob)
{
    qDebug() << "##### New source applied #####";
}

quint8 SingleImageHandler::getImageCount() const
{
    return 1;
}

char *SingleImageHandler::getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    if ( imageNumber > 0 )
    {
        qWarning() << "Only one image is available, requested image number: " << imageNumber;
        dataSize = 0;
        return nullptr;
    }
    try
    {
        Magick::Blob outputBlob;

        applyMaskFromClippingPath(*m_sourceImage, format);

        m_sourceImage->write(&outputBlob);

        dataSize = outputBlob.length();

        char* returnArray = new char[dataSize];
        memcpy(returnArray, static_cast<const char*>(outputBlob.data()), outputBlob.length());

        return returnArray;
    }
    catch( Magick::Exception &error)
    {
        qWarning() << Q_FUNC_INFO << "caught ImageMagick exception";
        qWarning() << error.what();
        dataSize = 0;
        return nullptr;
    }
}

void SingleImageHandler::handleSource()
{
    Magick::Image tmpImg;
    tmpImg.verbose(true);
    tmpImg.quiet(true);
    tmpImg.ping(*m_sourceBlob);
    qDebug() << "here...";
    if (tmpImg.density().isValid())
        qDebug() << "density: " << tmpImg.density().x() << "x" << tmpImg.density().y();
    qDebug() << "BOOM!";
    qDebug() << "resolution units: " << tmpImg.resolutionUnits();
    qDebug() << "BOOM!!";
    tmpImg.read(*m_sourceBlob);
    m_sourceImage.reset(new Magick::Image(tmpImg));
}

void SingleImageHandler::collectImageInfo()
{
    if (m_sourceImage == nullptr)
    {
        qWarning() << Q_FUNC_INFO << "source image uninitialized";
        return;
    }
    m_imageInfo.reset(new ImageInfo(*m_sourceImage));
}

void SingleImageHandler::applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format)
{
    try
    {
        image.alphaChannel(MagickCore::TransparentAlphaChannel);
        image.clip();
        image.alphaChannel(MagickCore::OpaqueAlphaChannel);

        image.magick(common::EImageFormatString[format]);
    }
    catch( Magick::Exception &error)
    {
        qWarning() << Q_FUNC_INFO << "caught ImageMagick exception";
        qWarning() << error.what();
    }
}


}
}
