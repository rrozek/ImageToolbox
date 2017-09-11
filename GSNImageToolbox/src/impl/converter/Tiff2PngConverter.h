#pragma once

#include "IConverter.h"

namespace GSNImageToolBox
{
namespace converter
{

class Tiff2PngConverter : public IConverter
{
public:
    Tiff2PngConverter();

    bool convert(Magick::Image& sourceImg, Magick::Blob& outputData) override {return true;}
};

}
}
