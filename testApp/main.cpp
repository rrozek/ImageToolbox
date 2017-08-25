#include <QCoreApplication>

#include "ToolBox.h"

#include <QFile>
#include <QByteArray>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GSNImageToolBox::ToolBox toolbox;
    QFile img("test.tiff");
    img.open(QIODevice::ReadOnly);
    QByteArray imgData = img.readAll();
    img.close();
    toolbox.setSource(imgData.data(), imgData.size());
    const char* data = toolbox.getImage(GSNImageToolBox::Common::PNG);

    QFile outimg("test.png");
    outimg.open(QIODevice::WriteOnly | QIODevice::Truncate);
    outimg.write(data);
    outimg.close();

    return a.exec();
}
