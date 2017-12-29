#include "IHandler.h"

#include "Magick++.h"

#include <QDebug>
#include <QByteArray>
#include <QFile>

namespace GSNImageToolBox
{
namespace handlers
{

IHandler::IHandler(const QJsonDocument &jsonImageInfo)
    : m_imageInfo(std::make_unique<ImageInfo>())
{
    m_imageInfo->loadJson(jsonImageInfo);
}
IHandler::IHandler(std::shared_ptr<Magick::Blob> blob, const QJsonDocument &jsonImageInfo)
    : m_sourceBlob(blob)
    , m_imageInfo(std::make_unique<ImageInfo>())
{
    m_imageInfo->loadJson(jsonImageInfo);
}

IHandler::~IHandler()
{
}

void IHandler::init()
{
    handleSource();
}

void IHandler::printImageInfo() const
{
    if (m_imageInfo == nullptr)
    {
        qWarning() << Q_FUNC_INFO << "image info uninitialized";
        return;
    }
    m_imageInfo->print();
}

const ImageInfo &IHandler::getImageInfo() const
{
    if (m_imageInfo == nullptr)
    {
        qWarning() << Q_FUNC_INFO << "image info uninitialized";
        return ImageInfo::invalid;
    }
    return *m_imageInfo;
}

char *IHandler::getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    return getThumbnail(m_imageInfo->getValue("root[" + QString::number(imageNumber) + "].image.geometry.width").toInt() * thumbPercentSize
                        , m_imageInfo->getValue("root[" + QString::number(imageNumber) + "].image.geometry.width").toInt() * thumbPercentSize
                        , imageNumber, format, dataSize);
}

char *IHandler::getThumbnail(float thumbPercentSize, quint8 imageNumber, size_t &dataSize)
{
    return getThumbnail(m_imageInfo->getValue("root[" + QString::number(imageNumber) + "].image.geometry.width").toInt() * thumbPercentSize
                        , m_imageInfo->getValue("root[" + QString::number(imageNumber) + "].image.geometry.width").toInt() * thumbPercentSize
                        , imageNumber, dataSize);
}

bool IHandler::applyIccProfile(Magick::Image &image, QString profileFilePath)
{
    QFile profileFile(profileFilePath);
    if (!profileFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open " << profileFile.fileName() << " profile file";
        return false;
    }
    QByteArray profileData = profileFile.readAll();
    try
    {
        Magick::Blob profile((void *) profileData.data(), profileData.size());
        image.profile("icc", profile);
    }
    catch( Magick::Error &error)
    {
        qWarning() << Q_FUNC_INFO << "caught ImageMagick exception";
        qWarning() << error.what();
        return false;
    }
    return true;
}

}
}
