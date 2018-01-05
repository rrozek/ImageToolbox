#include "ImageManipulationToolbox.h"
#include "ui_ImageManipulationToolbox.h"

#include "FileConverter.h"
#include <QThreadPool>
#include <QDebug>

ImageManipulationToolbox::ImageManipulationToolbox(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ImageManipulationToolbox)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton_png, &QPushButton::clicked, this, &ImageManipulationToolbox::slotButtonPngClicked);
    connect(m_ui->pushButton_tif, &QPushButton::clicked, this, &ImageManipulationToolbox::slotButtonTifClicked);
    connect(m_ui->pushButton_jpeg, &QPushButton::clicked, this, &ImageManipulationToolbox::slotButtonJpegClicked);
    m_ui->pushButton_tif->setEnabled(false);
    m_ui->pushButton_jpeg->setEnabled(false);
    m_ui->pushButton_png->setEnabled(false);
    setVisible(true);
}

void ImageManipulationToolbox::setSettings(std::shared_ptr<QSettings> settings)
{
    m_settings = settings;
}

void ImageManipulationToolbox::slotUpdateViewToFile(const QFileInfo &fileInfo)
{
    m_fileList.clear();
    m_fileList.append(fileInfo);
    updateViewToFile();
}

void ImageManipulationToolbox::slotUpdateViewToFile(const QFileInfoList &fileInfos)
{
    m_fileList.clear();
    m_fileList.append(fileInfos);
    updateViewToFile();
}

void ImageManipulationToolbox::scheduleFileConversion(const QFileInfo &fileInfo, GSNImageToolBox::common::EImageFormat targetFormat)
{
    emit signalBusy();
    FileConverter* converter = new FileConverter(fileInfo, targetFormat);
    connect(converter, &FileConverter::signalFinished, this, &ImageManipulationToolbox::slotFileConverted);
    QThreadPool::globalInstance()->start(converter);
}

void ImageManipulationToolbox::scheduleFileConversion(const QFileInfoList &fileInfos, GSNImageToolBox::common::EImageFormat targetFormat)
{
    for ( const QFileInfo& fileInfo : fileInfos )
    {
        scheduleFileConversion(fileInfo, targetFormat);
    }
}

void ImageManipulationToolbox::updateViewToFile()
{
    QList<GSNImageToolBox::common::EImageFormat> imageFormats;
    for (QFileInfo fileInfo : m_fileList)
    {
        GSNImageToolBox::common::EImageFormat fileFormat = GSNImageToolBox::common::EImageFormatFromString(fileInfo.suffix());
        if (fileFormat != GSNImageToolBox::common::COUNT && !imageFormats.contains(fileFormat))
            imageFormats.append(fileFormat);
    }

    m_ui->pushButton_tif->setEnabled(false);
    m_ui->pushButton_jpeg->setEnabled(false);
    m_ui->pushButton_png->setEnabled(false);
    if (imageFormats.contains(GSNImageToolBox::common::TIFF))
    {
        m_ui->pushButton_jpeg->setEnabled(true);
        m_ui->pushButton_png->setEnabled(true);
    }
    if (imageFormats.contains(GSNImageToolBox::common::PNG))
    {
        m_ui->pushButton_jpeg->setEnabled(true);
    }
    if (imageFormats.contains(GSNImageToolBox::common::JPEG))
    {
        m_ui->pushButton_png->setEnabled(true);
    }
    if (imageFormats.contains(GSNImageToolBox::common::EPS))
    {
        m_ui->pushButton_tif->setEnabled(true);
        m_ui->pushButton_jpeg->setEnabled(true);
        m_ui->pushButton_png->setEnabled(true);
    }

    // now to comply with settings
    m_settings->beginGroup("SupportedFormats");
    if (!m_settings->value("TIF").toBool())
        m_ui->pushButton_tif->setEnabled(false);
    if (!m_settings->value("JPEG").toBool())
        m_ui->pushButton_jpeg->setEnabled(false);
    if (!m_settings->value("PNG").toBool())
        m_ui->pushButton_png->setEnabled(false);
    m_settings->endGroup();
}

void ImageManipulationToolbox::slotFileConverted(const QFileInfo &fileInfo, bool success)
{
    if (!success)
    {
        qWarning() << "Converting: " << fileInfo.absoluteFilePath() << "failed";
    }
    m_fileList.removeOne(fileInfo);
    if (m_fileList.isEmpty())
        emit signalReady();
}

void ImageManipulationToolbox::slotButtonPngClicked()
{
    scheduleFileConversion(GSNImageToolBox::common::PNG);
}

void ImageManipulationToolbox::slotButtonTifClicked()
{
    scheduleFileConversion(GSNImageToolBox::common::TIFF);
}

void ImageManipulationToolbox::slotButtonJpegClicked()
{
    scheduleFileConversion(GSNImageToolBox::common::JPEG);
}

void ImageManipulationToolbox::scheduleFileConversion(GSNImageToolBox::common::EImageFormat targetFormat)
{
    scheduleFileConversion(m_fileList, targetFormat);
}
