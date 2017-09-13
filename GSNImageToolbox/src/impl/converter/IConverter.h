#pragma once
#include "Magick++.h"

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

    virtual bool convert(Magick::Image& sourceImg, Magick::Blob& outputData) = 0;
};

}
}
