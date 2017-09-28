#include "SingleImageHandler.h"

#include "Magick++.h"
#include "MagickCore/MagickCore.h"
#include <QDebug>
#include <QFile>

namespace GSNImageToolBox
{
namespace handlers
{

SingleImageHandler::SingleImageHandler(Magick::Blob &blob, const QJsonDocument &jsonImageInfo)
    : IHandler(blob, jsonImageInfo)
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
//        m_sourceImage->verbose(true);
//        Magick::Blob imageProfiles = m_sourceImage->profile("ICM");
//        qDebug() << QByteArray::fromBase64(QByteArray::fromStdString(imageProfiles.base64()));

//        GetPPException;

//        GetImageProperty(m_sourceImage->constImage(),"exif:*",exceptionInfo);
//        GetImageProperty(m_sourceImage->constImage(),"icc:*",exceptionInfo);
//        GetImageProperty(m_sourceImage->constImage(),"iptc:*",exceptionInfo);
//        GetImageProperty(m_sourceImage->constImage(),"xmp:*",exceptionInfo);
//        ResetImagePropertyIterator(m_sourceImage->constImage());
//        const char* property = GetNextImageProperty(m_sourceImage->constImage());
//        while ( property != static_cast<const char*>(NULL))
//        {
//            const char* value = GetImageProperty(m_sourceImage->constImage(),property,exceptionInfo);
//            qDebug() << property << value;
//            property = GetNextImageProperty(m_sourceImage->constImage());
//        }
        m_sourceImage->fileName("json:");
        m_sourceImage->write(&outputBlob);

        dataSize = outputBlob.length();

        char* returnArray = new char[dataSize];
        memcpy(returnArray, static_cast<const char*>(outputBlob.data()), outputBlob.length());
        QFile file("identify.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        file.write(returnArray);
        file.close();
//        qDebug() << returnArray;
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
