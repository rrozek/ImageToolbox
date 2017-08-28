#include <QCoreApplication>
#include <QDebug>

#include "ToolBox.h"

#include <QFile>
#include <QByteArray>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GSNImageToolBox::ToolBox toolbox;
    QFile img("test.tiff");
    if (!img.open(QIODevice::ReadOnly))
    {
        qDebug() << "cannot open file: " << img.fileName() << "reason, " << img.errorString();
        return -1;
    }
    QByteArray imgData = img.readAll();
    img.close();
    toolbox.setSource(imgData.data(), imgData.size());
    size_t dataLength = 0;
    char* dataPng = toolbox.getImage(GSNImageToolBox::Common::PNG, dataLength);

    QFile imgPng("test.png");
    if (!imgPng.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "cannot open file: " << imgPng.fileName() << "reason, " << imgPng.errorString();
        return -1;
    }
    if (imgPng.write(dataPng, dataLength) == -1)
    {
        qDebug() << imgPng.errorString();
    }
    imgPng.close();

    QByteArray arrayEps = toolbox.getImage(GSNImageToolBox::Common::EPS);

    QFile imgEps("test.eps");
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

    return a.exec();
}
