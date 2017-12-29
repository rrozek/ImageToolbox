#pragma once

#include "IHandler.h"
#include <QElapsedTimer>

namespace GSNImageToolBox
{
namespace handlers
{

class SingleImageHandler : public IHandler
{
public:
    SingleImageHandler(const QJsonDocument& jsonImageInfo);
    SingleImageHandler(std::shared_ptr<Magick::Blob> blob, const QJsonDocument& jsonImageInfo);
    ~SingleImageHandler();
    quint8 getImageCount() const override;
    char* getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize) override;

    char *getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, size_t &dataSize) override;
    char *getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, size_t &dataSize) override;

protected:
    void handleSource() override;

    void applyMaskFromClippingPath(Magick::Image &image, common::EImageFormat format) override;
    void applyCMYKToRGBProfiles(Magick::Image &image, common::EImageFormat format) override;

private:
    std::unique_ptr<Magick::Image> m_sourceImage;
    QElapsedTimer m_timerPerformance;

};

}
}
