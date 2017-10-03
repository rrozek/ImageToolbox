#include "ToolBox.h"

#include "Magick++.h"

#include <QDir>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "src/impl/handlers/SingleImageHandler.h"
//#include "src/impl/handlers/MultiPageImageHandler.h"

namespace GSNImageToolBox
{

ToolBox::ToolBox()
{
    Magick::InitializeMagick(QDir::currentPath().toStdString().c_str());
}

ToolBox::~ToolBox()
{
    qDebug() << Q_FUNC_INFO;
}

void ToolBox::setSource(const char *data, size_t size)
{
    qDebug() << "##### applying new source... #####";
    auto sourceBlob = std::make_shared<Magick::Blob>(data, size);

    Magick::Image sourceImage;
    Magick::Blob jsonMetadataRaw;
    try
    {
        sourceImage.quiet(true);
        sourceImage.verbose(true);
        sourceImage.ping(*sourceBlob);
        sourceImage.fileName("json:");
        sourceImage.write(&jsonMetadataRaw);
    }
    catch( Magick::Exception &error)
    {
        qWarning() << "ImageMagick Exception: " << error.what();
        return;
    }
    QString jsonString = QString::fromLocal8Bit(static_cast<const char*>(jsonMetadataRaw.data()), jsonMetadataRaw.length());
    QJsonParseError error;
    QJsonDocument jsonMetadata = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    if (jsonMetadata.isNull())
    {
        qWarning() << "invalid input json structure. Error: " << error.errorString() << "at position: " << error.offset;
        return;
    }
    bool isMultiImage = false;

    if (jsonMetadata.isArray())
    {
        if (jsonMetadata.array().size() > 1)
            isMultiImage = true;
        else
            isMultiImage = false;
    }
    else if (jsonMetadata.isObject())
        isMultiImage = false;
    else
    {
        qWarning() << "neither object nor array detected. invalid json file?";
        return;
    }

    qDebug() << "final verdict - is multi-image: " << isMultiImage;

    if (isMultiImage)
    {
        // TODO: Mikopson will do that
//        m_handler.reset(new handlers::MultiPageImageHandler(sourceBlob, jsonMetadata));
        qWarning() << "multi image not supported yet";
        return;
    }
    else
        m_handler.reset(new handlers::SingleImageHandler(sourceBlob, jsonMetadata));
    qDebug() << "goto init";
    m_handler->init();
    qDebug() << "goto printImageInfo";
    m_handler->printImageInfo();
}

void ToolBox::getImage(common::EImageFormat format, QByteArray& dataArray)
{
    getImage(0, format, dataArray);
}

void ToolBox::getImage(quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray)
{
    size_t dataSize = 0;
    char* rawArray = getImage(imageNumber, format, dataSize);
    dataArray = QByteArray(rawArray, dataSize);
    delete[] rawArray;
}

char* ToolBox::getImage(common::EImageFormat format, size_t& dataSize)
{
    return getImage(0, format, dataSize);
}

char *ToolBox::getImage(quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{

    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        dataSize = 0;
        return nullptr;
    }
    return m_handler->getImage(imageNumber, format, dataSize);
}

quint8 ToolBox::getImageCount() const
{

    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        return 0;
    }
    return m_handler->getImageCount();
}

void ToolBox::printImageInfo()
{
    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        return;
    }
    m_handler->printImageInfo();
}

const ImageInfo& ToolBox::getImageInfo() const
{

    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        return ImageInfo();
    }
    return m_handler->getImageInfo();
}

} // namespace GSNImageToolBox
