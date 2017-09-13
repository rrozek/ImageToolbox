#include "IHandler.h"

#include "Magick++.h"

#include <QDebug>

namespace GSNImageToolBox
{
namespace handlers
{

IHandler::IHandler(Magick::Blob& blob)
    : m_sourceBlob(std::make_shared<Magick::Blob>(blob))
{

}

IHandler::~IHandler()
{

}

void IHandler::init()
{
    handleSource();
    collectImageInfo();
}

void IHandler::printImageInfo() const
{
    m_imageInfo->print();
}

const ImageInfo &IHandler::getImageInfo() const
{
    if (m_imageInfo == nullptr)
    {
        qWarning() << Q_FUNC_INFO << "image info uninitialized";
        return ImageInfo();
    }
    return *m_imageInfo;
}

}
}
