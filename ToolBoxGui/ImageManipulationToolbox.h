#pragma once

#include <QWidget>
#include <QFileInfoList>

#include "Common.h"

namespace Ui {
class ImageManipulationToolbox;
}

class ImageManipulationToolbox : public QWidget
{
    Q_OBJECT
public:
    explicit ImageManipulationToolbox(QWidget *parent = nullptr);

    void slotUpdateViewToFile(const QFileInfo &fileInfo);
    void slotUpdateViewToFile(const QFileInfoList& fileInfos);
signals:
    void signalBusy();
    void signalReady();

private:
    void updateViewToFile();

    void slotFileConverted(const QFileInfo &fileInfo, bool success);

    void slotButtonAnyToPngClicked();
    void slotButtonEPSToPngClicked();
    void slotButtonEPSToTifClicked();
    void slotButtonTifToPngClicked();

    void scheduleFileConversion(GSNImageToolBox::common::EImageFormat targetFormat);

    Ui::ImageManipulationToolbox *m_ui;

    QFileInfoList m_fileList;
};

