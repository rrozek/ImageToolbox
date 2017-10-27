#include "ToolBoxGui.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QStyleFactory>

#include "ToolBox.h"

int main(int argc, char *argv[])
{
//    qputenv("MAGICK_CODER_MODULE_PATH", QString(QDir::currentPath() + "/modules/coders").toLocal8Bit());
//    qputenv("MAGICK_CODER_FILTER_PATH", QString(QDir::currentPath() + "/modules/filters").toLocal8Bit());
//    qputenv("MAGICK_CONFIGURE_PATH", QString(QDir::currentPath()).toLocal8Bit());
//    qputenv("LD_LIBRARY_PATH", QString(QDir::currentPath()).toLocal8Bit());

    QApplication a(argc, argv);

    qDebug() << QStyleFactory::keys();

    a.setStyle(QStyleFactory::create("Fusion"));

    GSNImageToolBox::ToolBox::InitializeMagickEnvironment();

    ToolBoxGui w;
    w.show();

    return a.exec();
}
