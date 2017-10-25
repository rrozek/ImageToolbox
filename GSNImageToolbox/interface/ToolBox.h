#pragma once

#include "gsnimagetoolbox_global.h"

//#include "ImageMagick/Magick++/Image.h"

#include "ImageInfo.h"

#include <memory>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace GSNImageToolBox
{
namespace handlers
{
class IHandler;
}
}

namespace Magick
{
class Blob;
class Image;
}

namespace GSNImageToolBox
{


class GSNIMAGETOOLBOXSHARED_EXPORT ToolBox
{

public:
    static void InitializeMagickEnvironment();

    ToolBox();
    ~ToolBox();

    QJsonDocument pingSource(const QString& absFilePath);
    void setSource(const QString& absFilePath);
//    void setSource(const char* data, size_t size); // deprecated due to performace issues

    void getMetadataJSON(QByteArray& jsonMetaData) const;

    void getImage(common::EImageFormat format, QByteArray &dataArray);
    void getImage(quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray);
    char* getImage(common::EImageFormat format, size_t &dataSize);
    char* getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize);

    // modify file format
    void getThumbnail(float thumbPercentSize, common::EImageFormat format, QByteArray &dataArray);
    void getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray);
    char* getThumbnail(float thumbPercentSize, common::EImageFormat format, size_t &dataSize);
    char* getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, size_t &dataSize);
    void getThumbnail(quint32 cropToWidth, quint32 cropToHeight, common::EImageFormat format, QByteArray &dataArray);
    void getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray);
    char* getThumbnail(quint32 cropToWidth, quint32 cropToHeight, common::EImageFormat format, size_t &dataSize);
    char* getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, size_t &dataSize);

    // don't modify file format
    void getThumbnail(float thumbPercentSize, QByteArray &dataArray);
    void getThumbnail(float thumbPercentSize, quint8 imageNumber, QByteArray &dataArray);
    char* getThumbnail(float thumbPercentSize, size_t &dataSize);
    char* getThumbnail(float thumbPercentSize, quint8 imageNumber, size_t &dataSize);
    void getThumbnail(quint32 cropToWidth, quint32 cropToHeight, QByteArray &dataArray);
    void getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, QByteArray &dataArray);
    char* getThumbnail(quint32 cropToWidth, quint32 cropToHeight, size_t &dataSize);
    char* getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, size_t &dataSize);

    quint8 getImageCount() const;

    void printImageInfo();
    const ImageInfo& getImageInfo() const;

    static bool magickInitialized;

private:
    void prepareSource(const QString& absFilePath, Magick::Image& sourceImage, QJsonDocument& jsonMetadata, bool& isMultiImage);
    bool readFile(const QString& absFilePath, QByteArray& targetDataArray);
    bool applyMaskFromClippingPath(Magick::Image& manipulatedImg, common::EImageFormat format);

    std::unique_ptr<handlers::IHandler> m_handler;

};

} // namespace GSNImageToolBox
