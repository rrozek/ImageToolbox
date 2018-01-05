#include "ToolBoxGui.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QStyleFactory>

#include "ToolBox.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    GSNImageToolBox::ToolBox::InitializeMagickEnvironment();

    QString startDirectory;
    if (a.arguments().size() > 1)
        startDirectory = a.arguments()[1];
    ToolBoxGui w(startDirectory);
    w.show();

    return a.exec();
}
