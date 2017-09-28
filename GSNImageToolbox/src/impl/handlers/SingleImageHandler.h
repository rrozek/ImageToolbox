#pragma once

#include "IHandler.h"

namespace GSNImageToolBox
{
namespace handlers
{

class SingleImageHandler : public IHandler
{
public:
    SingleImageHandler(Magick::Blob &blob);

    quint8 getImageCount() const override;
    char* getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize) override;

protected:
    void handleSource() override;

    void applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format) override;

private:
    std::unique_ptr<Magick::Image> m_sourceImage;

};

}
}
