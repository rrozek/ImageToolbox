#include "ToolBox.h"

#include "Magick++.h"

#include "StdCapture.h"

#include <QDir>
#include <QDebug>
#include <QRegularExpression>

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

}

void ToolBox::setSource(const char *data, size_t size)
{
    qDebug() << "##### applying new source... #####";
    Magick::Blob sourceBlob(data, size);

    Magick::Image sourceImage;
    StdCapture captureStd;

    try
    {
        captureStd.BeginCapture();

        sourceImage.verbose(true);
        sourceImage.quiet(true);
        sourceImage.ping(sourceBlob);

        captureStd.EndCapture();
    }
    catch( Magick::Exception &error)
    {
        captureStd.EndCapture();
        qDebug() << captureStd.GetCapture().c_str();
        qWarning() << "ImageMagick Exception: " << error.what();
        return;
    }
    catch(...)
    {
        captureStd.EndCapture();
        qDebug() << captureStd.GetCapture().c_str();
        qWarning() << "Exception thrown...";
        return;
    }

    QStringList pingLines = QString::fromStdString(captureStd.GetCapture()).split("\n", QString::KeepEmptyParts);
    qDebug() << "captured: " << pingLines;
    QRegularExpression regex;
    regex.setPattern("^\\[\\d*\\]");

    quint8 pagesCount = 0;
    bool isMultiImage = false;

    for ( QString line : pingLines )
    {
        if (line.contains(regex))
            pagesCount++;
    }
    if (pagesCount > 1)
        isMultiImage = true;

    qDebug() << "Detected: " << pagesCount << "images";
    qDebug() << "final verdict - is multi-image: " << isMultiImage;

    if (isMultiImage)
    {
//        m_handler.reset(new handlers::MultiPageImageHandler(sourceBlob));
        qWarning() << "multi image not supported yet";
        return;
    }
    else
        m_handler.reset(new handlers::SingleImageHandler(sourceBlob));

    m_handler->init();
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
