#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include "ToolBox.h"

#include <QFile>
#include <QByteArray>

#include <QTimer>

void printUsage();

void Tiff2Png(const QFileInfo& info);
void Tiff2Eps(const QFileInfo& info);
void Tiff2Eps2Png(const QFileInfo& info);
void Eps2Png(const QFileInfo& info);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList params = a.arguments();
    if (params.size() != 2)
    {
        printUsage();
        return -1;
    }

    QDir usedDir(params[1]);
    if (!usedDir.exists() || !usedDir.isReadable())
    {
        qWarning() << "directory: " << params[1] << "does not exist or is unreadable";
        return -2;
    }

    usedDir.setFilter(QDir::Files | QDir::Readable);
    QStringList fileFilters;
    fileFilters << "*.tif";
    fileFilters << "*.tiff";
    fileFilters << "*.TIF";
    fileFilters << "*.TIFF";
    QFileInfoList sourceTifFiles = usedDir.entryInfoList(fileFilters);

    usedDir.mkdir("Tiff2Png");
    usedDir.mkdir("Tiff2Eps");
    usedDir.mkdir("Tiff2Eps2Png");
    for ( QFileInfo info : sourceTifFiles )
    {
        Tiff2Png(info);
        Tiff2Eps(info);
        Tiff2Eps2Png(info);
    }

    fileFilters.clear();

    fileFilters << "*.eps";
    fileFilters << "*.EPS";
    QFileInfoList sourceEpsFiles = usedDir.entryInfoList(fileFilters);

    usedDir.mkdir("Eps2Png");
    for ( QFileInfo info : sourceEpsFiles )
    {
        Eps2Png(info);
    }

    QTimer::singleShot(0, Qt::CoarseTimer, &a, &QCoreApplication::quit);
    return a.exec();
}

void printUsage()
{
    qDebug() << "Hi. This is test app. It takes a directory as an input";
    qDebug() << "and processes all *.tif *.tiff *.eps images.";
    qDebug() << "As a result it creates 4 subdirectories: Tiff2Png, Tiff2Eps Tiff2Eps2Png and Eps2Png";
    qDebug() << "Processed files land in specific directories. Thats all, have fun!";
    qDebug() << "######################################################";
    qDebug() << "#### usage: testApp <abs_path_to_input_directory> ####";
    qDebug() << "######################################################";
}

void readFile(const QString& absFilePath, QByteArray& targetDataArray)
{
    qDebug() << "read from: " << absFilePath;
    QFile img(absFilePath);
    img.open(QIODevice::ReadOnly);
    targetDataArray = img.readAll();
    img.close();
}

void writeFile(const QString& absFilePath, const QByteArray& result)
{
    qDebug() << "write to: " << absFilePath;
    QFile img(absFilePath);
    img.open(QIODevice::WriteOnly | QIODevice::Truncate);
    img.write(result);
    img.close();
}

void processFile(const QFileInfo& info, GSNImageToolBox::common::EImageFormat format)
{
    GSNImageToolBox::ToolBox toolbox;

    QByteArray data;
    readFile(info.absoluteFilePath(), data);

    toolbox.setSource(data.data(), data.length());

    QByteArray metadata;
    toolbox.getMetadataJSON(metadata);
    writeFile(info.absoluteFilePath() + "_metadata.json", metadata);

    QByteArray result;
    toolbox.getImage(format, result);

    QString subdir;
    if (info.completeSuffix().contains("tif", Qt::CaseInsensitive) ||
        info.completeSuffix().contains("tiff", Qt::CaseInsensitive))
        subdir = "/Tiff2";
    if (info.completeSuffix().contains("eps", Qt::CaseInsensitive))
        subdir = "/Eps2";

    subdir.append(GSNImageToolBox::common::EImageFormatStringCapital[format]);
    subdir.append("/");

    writeFile(info.absoluteDir().absolutePath() + subdir + info.baseName() + "." + QString(GSNImageToolBox::common::EImageFormatString[format]), result);

}

void Tiff2Png(const QFileInfo& info)
{
    processFile(info, GSNImageToolBox::common::PNG);
}

void Tiff2Eps(const QFileInfo& info)
{
    processFile(info, GSNImageToolBox::common::EPS);
}

void Tiff2Eps2Png(const QFileInfo& info)
{
    GSNImageToolBox::ToolBox toolbox;

    QByteArray data;
    readFile(info.absoluteFilePath(), data);

    toolbox.setSource(data.data(), data.length());

    QByteArray result;

    toolbox.getImage(GSNImageToolBox::common::EPS, result);

    toolbox.setSource(result.data(), result.length());

    QByteArray finalResult;
    toolbox.getImage(GSNImageToolBox::common::PNG, finalResult);

    writeFile(info.absoluteDir().absolutePath() + "/Tiff2Eps2Png/" + info.baseName() + "." + GSNImageToolBox::common::EImageFormatString[GSNImageToolBox::common::PNG], finalResult);
}

void Eps2Png(const QFileInfo& info)
{
    processFile(info, GSNImageToolBox::common::PNG);
}
