#pragma once
#include "ImageMagick/Magick++.h"

#include "Common.h"

namespace GSNImageToolBox
{
namespace converter
{

class IConverter
{
public:
    IConverter();
    virtual ~IConverter();

    bool convert(Magick::Image& sourceImg, Magick::Blob& outputData) = 0;
};

}
}
