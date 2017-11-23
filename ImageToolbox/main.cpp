#include "ToolBoxGui.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QStyleFactory>

#include "ToolBox.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << QStyleFactory::keys();

    a.setStyle(QStyleFactory::create("Fusion"));

    GSNImageToolBox::ToolBox::InitializeMagickEnvironment();

    ToolBoxGui w;
    w.show();

    return a.exec();
}
