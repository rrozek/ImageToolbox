#pragma once

#include "IHandler.h"

namespace GSNImageToolBox
{
namespace handlers
{

class MultiPageImageHandler : public IHandler
{
public:
    MultiPageImageHandler(const QJsonDocument& jsonImageInfo);
    MultiPageImageHandler(std::shared_ptr<Magick::Blob> blob, const QJsonDocument& jsonImageInfo);

    quint8 getImageCount() const override;
    char *getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize) override;

    char *getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, size_t &dataSize) override;
    char *getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, size_t &dataSize) override;

protected:
    void handleSource() override;

    void applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format) override;
    void applyCMYKToRGBProfiles(Magick::Image &image, common::EImageFormat format) override;
};

}
}
