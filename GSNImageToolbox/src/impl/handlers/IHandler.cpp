#include "IHandler.h"

#include "Magick++.h"

#include <QDebug>

namespace GSNImageToolBox
{
namespace handlers
{

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

}
}
