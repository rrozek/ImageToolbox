#include "ToolBox.h"

#include "Magick++.h"

#include <QDir>
#include <QDirIterator>
#include <QLibrary>

#include <QDebug>

#include <QCoreApplication>
#include <QProcessEnvironment>
#include <QElapsedTimer>

#include "src/impl/handlers/SingleImageHandler.h"
//#include "src/impl/handlers/MultiPageImageHandler.h"

namespace GSNImageToolBox
{

bool ToolBox::magickInitialized = false;

ToolBox::ToolBox()
{
    if (!magickInitialized)
    {
        qFatal("You need to call InitializeMagickEnvironment() before any actions");
    }
}

ToolBox::~ToolBox()
{
}

QJsonDocument ToolBox::pingSource(const QString &absFilePath)
{
    Magick::Image sourceImage;
    QJsonDocument jsonMetadata;
    bool isMultiImage = false;

    prepareSource(absFilePath, sourceImage, jsonMetadata, isMultiImage);
    return jsonMetadata;
}

void ToolBox::setSource(const QString &absFilePath)
{
    Magick::Image sourceImage;
    QJsonDocument jsonMetadata;
    bool isMultiImage = false;

    prepareSource(absFilePath, sourceImage, jsonMetadata, isMultiImage);

    QByteArray content;
    readFile(absFilePath, content);
    auto sourceBlob = std::make_shared<Magick::Blob>(content.data(), content.length());

    if (isMultiImage)
    {
        // TODO: Mikopson will do that
//        m_handler.reset(new handlers::MultiPageImageHandler(sourceBlob, jsonMetadata));
        qWarning() << "multi image not supported yet";
        return;
    }
    else
        m_handler.reset(new handlers::SingleImageHandler(sourceBlob, jsonMetadata));
    m_handler->init();
}

void ToolBox::getMetadataJSON(QByteArray &jsonMetaData) const
{
    m_handler->getImageInfo().dumpTo(jsonMetaData);
}

void ToolBox::getImage(common::EImageFormat format, QByteArray& dataArray)
{
    getImage(0, format, dataArray);
}

void ToolBox::getImage(quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray)
{
    size_t dataSize = 0;
    char* rawArray = getImage(imageNumber, format, dataSize);
    dataArray = QByteArray(rawArray, static_cast<int>(dataSize));
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

void ToolBox::getThumbnail(float thumbPercentSize, common::EImageFormat format, QByteArray &dataArray)
{
    getThumbnail(thumbPercentSize, 0, format, dataArray);
}

void ToolBox::getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray)
{
    size_t dataSize = 0;
    char* rawArray = getThumbnail(thumbPercentSize, imageNumber, format, dataSize);
    dataArray = QByteArray(rawArray, static_cast<int>(dataSize));
    delete[] rawArray;
}

char *ToolBox::getThumbnail(float thumbPercentSize, common::EImageFormat format, size_t &dataSize)
{
    return getThumbnail(thumbPercentSize, 0, format, dataSize);
}

char *ToolBox::getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        dataSize = 0;
        return nullptr;
    }
    return m_handler->getThumbnail(thumbPercentSize, imageNumber, format, dataSize);
}

void ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, common::EImageFormat format, QByteArray &dataArray)
{
    getThumbnail(cropToWidth, cropToHeight, 0, format, dataArray);
}

void ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray)
{
    size_t dataSize = 0;
    char* rawArray = getThumbnail(cropToWidth, cropToHeight, imageNumber, format, dataSize);
    dataArray = QByteArray(rawArray, static_cast<int>(dataSize));
    delete[] rawArray;
}

char *ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, common::EImageFormat format, size_t &dataSize)
{
    return getThumbnail(cropToWidth, cropToHeight, 0, format, dataSize);
}

char *ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        dataSize = 0;
        return nullptr;
    }
    return m_handler->getThumbnail(cropToWidth, cropToHeight, imageNumber, format, dataSize);
}

void ToolBox::getThumbnail(float thumbPercentSize, QByteArray &dataArray)
{
    QElapsedTimer timer;
    timer.start();
    getThumbnail(thumbPercentSize, 0, dataArray);
    qWarning() << "Whole thumbnail generation took: " << timer.elapsed();
}

void ToolBox::getThumbnail(float thumbPercentSize, quint8 imageNumber, QByteArray &dataArray)
{
    size_t dataSize = 0;
    char* rawArray = getThumbnail(thumbPercentSize, imageNumber, dataSize);
    dataArray = QByteArray(rawArray, static_cast<int>(dataSize));
    delete[] rawArray;
}

char *ToolBox::getThumbnail(float thumbPercentSize, size_t &dataSize)
{
    return getThumbnail(thumbPercentSize, 0, dataSize);
}

char *ToolBox::getThumbnail(float thumbPercentSize, quint8 imageNumber, size_t &dataSize)
{
    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        dataSize = 0;
        return nullptr;
    }
    return m_handler->getThumbnail(thumbPercentSize, imageNumber, dataSize);
}

void ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, QByteArray &dataArray)
{
    QElapsedTimer timer;
    timer.start();
    getThumbnail(cropToWidth, cropToHeight, 0, dataArray);
    qWarning() << "Whole thumbnail generation took: " << timer.elapsed();
}

void ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, QByteArray &dataArray)
{
    size_t dataSize = 0;
    char* rawArray = getThumbnail(cropToWidth, cropToHeight, imageNumber, dataSize);
    dataArray = QByteArray(rawArray, static_cast<int>(dataSize));
    delete[] rawArray;}

char *ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, size_t &dataSize)
{
    return getThumbnail(cropToWidth, cropToHeight, 0, dataSize);
}

char *ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, size_t &dataSize)
{
    if (m_handler == nullptr)
    {
        qWarning() << "No image set so far";
        dataSize = 0;
        return nullptr;
    }
    return m_handler->getThumbnail(cropToWidth, cropToHeight, imageNumber, dataSize);
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
        return ImageInfo::invalid;
    }
    return m_handler->getImageInfo();
}

void ToolBox::prepareSource(const QString& absFilePath, Magick::Image& sourceImage, QJsonDocument& jsonMetadata, bool &isMultiImage)
{
    Magick::Blob jsonMetadataRaw;
    isMultiImage = false;
    try
    {
        sourceImage.quiet(true);
        sourceImage.verbose(true);
        sourceImage.ping(absFilePath.toStdString());
        sourceImage.fileName("json:");
        sourceImage.write(&jsonMetadataRaw);
    }
    catch( Magick::Error &error)
    {
        qWarning() << "abc ImageMagick Exception: " << error.what();
        return;
    }
    QString jsonString = QString::fromLocal8Bit(static_cast<const char*>(jsonMetadataRaw.data()), static_cast<int>(jsonMetadataRaw.length()));
    QJsonParseError error;
    jsonMetadata = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    if (jsonMetadata.isNull())
    {
        qWarning() << "invalid input json structure. Error: " << error.errorString() << "at position: " << error.offset;
        return;
    }

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
}

bool ToolBox::readFile(const QString& absFilePath, QByteArray& targetDataArray)
{
    qDebug() << "read from: " << absFilePath;
    QFile img(absFilePath);
    if (!img.open(QIODevice::ReadOnly))
    {
        qWarning() << "cannot open file " << absFilePath << "to read";
        return false;
    }
    targetDataArray = img.readAll();
    img.close();
    return true;
}

void ToolBox::InitializeMagickEnvironment()
{
    qDebug() << QCoreApplication::applicationDirPath();
    Magick::InitializeMagick(QCoreApplication::applicationDirPath().toStdString().c_str());
    ToolBox::magickInitialized = true;
}

} // namespace GSNImageToolBox
