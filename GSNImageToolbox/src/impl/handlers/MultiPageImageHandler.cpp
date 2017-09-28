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

void MultiPageImageHandler::applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format)
{

}

}
}
