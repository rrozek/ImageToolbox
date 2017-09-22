#include "MultiPageImageHandler.h"

namespace GSNImageToolBox
{
namespace handlers
{

MultiPageImageHandler::MultiPageImageHandler(Magick::Blob &blob)
    : IHandler(blob)
{

}

quint8 MultiPageImageHandler::getImageCount() const
{
    return 1;
}

char *MultiPageImageHandler::getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    return nullptr;
}

void MultiPageImageHandler::handleSource()
{

}

void MultiPageImageHandler::collectImageInfo()
{
//    if (m_sourceImage == nullptr)
//    {
//        qWarning() << Q_FUNC_INFO << "source image uninitialized";
//        return;
//    }
}

void MultiPageImageHandler::applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format)
{

}

}
}
