#include "FileConverter.h"
#include "ToolBox.h"

#include <QDebug>

FileConverter::FileConverter(const QFileInfo &fileInfo, GSNImageToolBox::common::EImageFormat targetFormat)
    : m_fileInfo(fileInfo)
    , m_targetFileFormat(targetFormat)

{

}

void FileConverter::run()
{
    GSNImageToolBox::ToolBox toolbox;
    QByteArray imageData;
    QString subdirName = "ToolBox_conver";

    m_fileInfo.dir().mkdir(subdirName);
    QString absTargetFilePath(m_fileInfo.absoluteDir().absolutePath() + "/" + subdirName + "/" + m_fileInfo.baseName() + "." + QString(GSNImageToolBox::common::EImageFormatString[m_targetFileFormat]));

    toolbox.setSource(m_fileInfo.absoluteFilePath());
    toolbox.getImage(m_targetFileFormat, imageData);

    QFile img(absTargetFilePath);
    if (!img.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "Unable to write file: " << absTargetFilePath;
        qWarning() << "reason: " << img.errorString();
        emit signalFinished(m_fileInfo, false);
        return;
    }
    img.write(imageData);
    img.close();

    emit signalFinished(m_fileInfo, true);
}
