#include "SingleImageHandler.h"

#include "Magick++.h"
#include "MagickCore/MagickCore.h"
#include <QDebug>
#include <QFile>

namespace GSNImageToolBox
{
namespace handlers
{

SingleImageHandler::SingleImageHandler(std::shared_ptr<Magick::Blob> blob, const QJsonDocument &jsonImageInfo)
    : IHandler(blob, jsonImageInfo)
{
    qDebug() << "##### New source applied #####";
}

SingleImageHandler::~SingleImageHandler()
{
    qDebug() << Q_FUNC_INFO;
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
        applyMaskFromClippingPath(*m_sourceImage,format);
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
    try
    {
        m_sourceImage = std::make_unique<Magick::Image>();
        m_sourceImage->density(Magick::Point(m_imageInfo->getValue("root[0].image.resolution.x").toInt(),m_imageInfo->getValue("root[0].image.resolution.y").toInt()));
        m_sourceImage->read(*m_sourceBlob, Magick::Geometry(m_imageInfo->getValue("root[0].image.geometry.width").toInt(),
                                                            m_imageInfo->getValue("root[0].image.geometry.height").toInt(),
                                                            m_imageInfo->getValue("root[0].image.geometry.x").toInt(),
                                                            m_imageInfo->getValue("root[0].image.geometry.y").toInt()
                                                            ), m_imageInfo->getValue("root[0].image.deptrh").toInt()
                            );
    }
    catch( Magick::Exception &error)
    {
        qWarning() << Q_FUNC_INFO << "caught ImageMagick exception";
        qWarning() << error.what();
    }
}

void SingleImageHandler::applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format)
{
    try
    {
        QVariant clipPath = m_imageInfo->getValue("root[0].image.clipping path");
        if (clipPath.isValid())
        {
        image.alphaChannel(MagickCore::TransparentAlphaChannel);
        image.clip();
        image.alphaChannel(MagickCore::OpaqueAlphaChannel);
        }
        else
            qDebug() << "no clip path present.";

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
