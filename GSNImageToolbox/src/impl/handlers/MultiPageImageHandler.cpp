#include "MultiPageImageHandler.h"

namespace GSNImageToolBox
{
namespace handlers
{

MultiPageImageHandler::MultiPageImageHandler(std::shared_ptr<Magick::Blob> blob, const QJsonDocument &jsonImageInfo)
    : IHandler(blob, jsonImageInfo)
{

}

quint8 MultiPageImageHandler::getImageCount() const
{
    return 1;
}

char *MultiPageImageHandler::getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    Q_UNUSED(imageNumber)
    Q_UNUSED(format)
    Q_UNUSED(dataSize)
    return nullptr;
}

void MultiPageImageHandler::handleSource()
{

}

void MultiPageImageHandler::applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format)
{
    Q_UNUSED(image)
    Q_UNUSED(format)
}

}
}
