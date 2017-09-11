#include <QCoreApplication>
#include <QDebug>

#include "ToolBox.h"

#include <QFile>
#include <QByteArray>

#include <QTimer>

void printUsage();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    QStringList params = a.arguments();
//    if (params.size() != 2)
//    {
//        printUsage();
//        return -1;
//    }
    GSNImageToolBox::ToolBox toolbox;
    QFile img("test.img");
    if (!img.open(QIODevice::ReadOnly))
    {
        qDebug() << "cannot open file: " << img.fileName() << "reason, " << img.errorString();
        return -1;
    }
    QByteArray imgData = img.readAll();
    img.close();
    toolbox.setSource(imgData.data(), imgData.size());
    toolbox.printImageInfo();
    // -------------------------------------------------------------------

    QByteArray arrayEps;
    GSNImageToolBox::common::EImageFormat requestedFormatEps = GSNImageToolBox::common::EPS;
    toolbox.getImage(requestedFormatEps, arrayEps);

    QFile imgEps("testTifToEps.eps");
    if (!imgEps.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "cannot open file: " << imgEps.fileName() << "reason, " << imgEps.errorString();
        return -1;
    }
    if (imgEps.write(arrayEps) == -1)
    {
        qDebug() << imgEps.errorString();
    }
    imgEps.close();

    // -------------------------------------------------------------------

    QByteArray arrayPng;
    GSNImageToolBox::common::EImageFormat requestedFormatPng = GSNImageToolBox::common::PNG;
    toolbox.getImage(requestedFormatPng, arrayPng);

    QFile imgPng("testTifToPng.png");
    if (!imgPng.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "cannot open file: " << imgPng.fileName() << "reason, " << imgPng.errorString();
        return -1;
    }
    if (imgPng.write(arrayPng) == -1)
    {
        qDebug() << imgPng.errorString();
    }
    imgPng.close();

    // -------------------------------------------------------------------

    if (!imgEps.open(QIODevice::ReadOnly))
    {
        qDebug() << "cannot open file: " << imgEps.fileName() << "reason, " << imgEps.errorString();
        return -1;
    }
    QByteArray imgEpsData = imgEps.readAll();
    qDebug() << "data size: " << imgEpsData.size();
    imgEps.close();
    toolbox.setSource(imgEpsData.data(), imgEpsData.size());

    // -------------------------------------------------------------------

    {
        QByteArray arrayPngFromEps;
        toolbox.getImage(requestedFormatPng, arrayPngFromEps);

        QFile imgPng("testTifToEpsToPng.png");
        if (!imgPng.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qDebug() << "cannot open file: " << imgPng.fileName() << "reason, " << imgPng.errorString();
            return -1;
        }
        if (imgPng.write(arrayPngFromEps) == -1)
        {
            qDebug() << imgPng.errorString();
        }
        imgPng.close();
    }


    QTimer::singleShot(0, Qt::CoarseTimer, &a, &QCoreApplication::quit);
    return a.exec();
}

void printUsage()
{
    qDebug() << "usage: testApp <abs_path_to_input_img>";
}
