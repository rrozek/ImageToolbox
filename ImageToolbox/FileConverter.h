#pragma once

#include <QObject>
#include <QRunnable>
#include <QFileInfo>
#include <QString>
#include <QDir>

#include "Common.h"

class FileConverter : public QObject, public QRunnable
{
    Q_OBJECT
public:
    FileConverter(const QFileInfo &fileInfo, GSNImageToolBox::common::EImageFormat targetFormat);
    void run() override;

signals:
    void signalFinished(const QFileInfo &fileInfo, bool success);
private:
    QFileInfo m_fileInfo;
    GSNImageToolBox::common::EImageFormat m_targetFileFormat;
};

