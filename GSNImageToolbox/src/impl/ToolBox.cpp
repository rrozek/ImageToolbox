#include "ToolBox.h"

#include "Magick++.h"

#include <QDir>
#include <QDirIterator>
#include <QLibrary>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QCoreApplication>
#include <QProcessEnvironment>

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

void ToolBox::setSource(const QString &absFilePath)
{
    Magick::Image sourceImage;
    Magick::Blob jsonMetadataRaw;
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

void ToolBox::getThumbnail(float thumbPercentSize, common::EImageFormat format, QByteArray &dataArray)
{

}

void ToolBox::getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray)
{

}

char *ToolBox::getThumbnail(float thumbPercentSize, common::EImageFormat format, size_t &dataSize)
{
    return nullptr;
}

char *ToolBox::getThumbnail(float thumbPercentSize, quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    return nullptr;
}

void ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, common::EImageFormat format, QByteArray &dataArray)
{

}

void ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, QByteArray &dataArray)
{

}

char *ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, common::EImageFormat format, size_t &dataSize)
{
    return nullptr;
}

char *ToolBox::getThumbnail(quint32 cropToWidth, quint32 cropToHeight, quint8 imageNumber, common::EImageFormat format, size_t &dataSize)
{
    return nullptr;
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
//    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

//    env.insert("MAGICK_CODER_MODULE_PATH", QCoreApplication::applicationDirPath().append("/magick/modules/coders"));
//    env.insert("MAGICK_CODER_FILTER_PATH", QCoreApplication::applicationDirPath().append("/magick/modules/filters"));
//    env.insert("LD_LIBRARY_PATH", QCoreApplication::applicationDirPath().append("/magick/lib"));
//    env.insert("MAGICK_CONFIGURE_PATH", QCoreApplication::applicationDirPath().append("/magick/configuration"));
//    qDebug() << QCoreApplication::libraryPaths();
//    QStringList envList = env.toStringList();
//    for ( QString singleEnv : envList )
//    {
//        qDebug() << "-----------";
//        qDebug() << singleEnv;
//    }

//    QString path;
//    QSet<QString> libraries;
//    QDirIterator it{QCoreApplication::applicationDirPath(), {"*.dll"}};
//    while (it.hasNext())
//      libraries << it.next();
//    bool progress = true;
//    while (progress) {
//      progress = false;
//      for (auto i = libraries.begin(); i != libraries.end();) {
//        QLibrary lib{*i};
//        if (lib.load()) {
//          progress = true;
//          i = libraries.erase(i);
//        } else
//          ++i;
//       }
//    }

    Magick::InitializeMagick(QCoreApplication::applicationDirPath().toStdString().c_str());
    ToolBox::magickInitialized = true;
}

} // namespace GSNImageToolBox
