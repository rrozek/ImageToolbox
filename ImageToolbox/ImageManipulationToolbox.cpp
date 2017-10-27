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
    connect(m_ui->pushButton_anyToPng, &QPushButton::clicked, this, &ImageManipulationToolbox::slotButtonAnyToPngClicked);
    connect(m_ui->pushButton_epsToPng, &QPushButton::clicked, this, &ImageManipulationToolbox::slotButtonEPSToPngClicked);
    connect(m_ui->pushButton_epsToTif, &QPushButton::clicked, this, &ImageManipulationToolbox::slotButtonEPSToTifClicked);
    connect(m_ui->pushButton_tifToPng, &QPushButton::clicked, this, &ImageManipulationToolbox::slotButtonTifToPngClicked);
    setVisible(false);
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

void ImageManipulationToolbox::updateViewToFile()
{
    setVisible(true);
    QList<GSNImageToolBox::common::EImageFormat> imageFormats;
    for (QFileInfo fileInfo : m_fileList)
    {
        GSNImageToolBox::common::EImageFormat fileFormat = GSNImageToolBox::common::EImageFormatFromString(fileInfo.suffix());
        if (fileFormat != GSNImageToolBox::common::COUNT && !imageFormats.contains(fileFormat))
            imageFormats.append(fileFormat);
    }
    if (imageFormats.size() > 1)
    {
        m_ui->pushButton_anyToPng->setVisible(true);
        m_ui->pushButton_epsToPng->setVisible(false);
        m_ui->pushButton_epsToTif->setVisible(false);
        m_ui->pushButton_tifToPng->setVisible(false);
    }
    else
    {
        m_ui->pushButton_anyToPng->setVisible(false);
        m_ui->pushButton_epsToPng->setVisible(false);
        m_ui->pushButton_epsToTif->setVisible(false);
        m_ui->pushButton_tifToPng->setVisible(false);
        if (imageFormats.contains(GSNImageToolBox::common::TIFF))
        {
            m_ui->pushButton_tifToPng->setVisible(true);
        }
        if (imageFormats.contains(GSNImageToolBox::common::EPS))
        {
            m_ui->pushButton_epsToTif->setVisible(true);
            m_ui->pushButton_epsToPng->setVisible(true);
        }
    }
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

void ImageManipulationToolbox::slotButtonAnyToPngClicked()
{
    scheduleFileConversion(GSNImageToolBox::common::PNG);
}

void ImageManipulationToolbox::slotButtonEPSToPngClicked()
{
    scheduleFileConversion(GSNImageToolBox::common::PNG);
}

void ImageManipulationToolbox::slotButtonEPSToTifClicked()
{
    scheduleFileConversion(GSNImageToolBox::common::TIFF);
}

void ImageManipulationToolbox::slotButtonTifToPngClicked()
{
    scheduleFileConversion(GSNImageToolBox::common::PNG);
}

void ImageManipulationToolbox::scheduleFileConversion(GSNImageToolBox::common::EImageFormat targetFormat)
{
    for ( QFileInfo fileInfo : m_fileList )
    {
        emit signalBusy();
        FileConverter* converter = new FileConverter(fileInfo, targetFormat);
        connect(converter, &FileConverter::signalFinished, this, &ImageManipulationToolbox::slotFileConverted);
        QThreadPool::globalInstance()->start(converter);
    }
}
