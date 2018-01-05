#pragma once

#include <QWidget>
#include <QFileInfoList>
#include <QSettings>
#include <memory>

#include "Common.h"

namespace Ui {
class ImageManipulationToolbox;
}

class ImageManipulationToolbox : public QWidget
{
    Q_OBJECT
public:
    explicit ImageManipulationToolbox(QWidget *parent = nullptr);

    void setSettings(std::shared_ptr<QSettings> settings);

    void slotUpdateViewToFile(const QFileInfo &fileInfo);
    void slotUpdateViewToFile(const QFileInfoList& fileInfos);

    void scheduleFileConversion(const QFileInfo& fileInfo, GSNImageToolBox::common::EImageFormat targetFormat);
    void scheduleFileConversion(const QFileInfoList& fileInfos, GSNImageToolBox::common::EImageFormat targetFormat);

signals:
    void signalBusy();
    void signalReady();

private:
    void updateViewToFile();

    void slotFileConverted(const QFileInfo &fileInfo, bool success);

    void slotButtonPngClicked();
    void slotButtonTifClicked();
    void slotButtonJpegClicked();

    void scheduleFileConversion(GSNImageToolBox::common::EImageFormat targetFormat);

    Ui::ImageManipulationToolbox *m_ui;

    std::shared_ptr<QSettings> m_settings;
    QFileInfoList m_fileList;
};

