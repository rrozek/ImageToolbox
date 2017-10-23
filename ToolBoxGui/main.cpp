#include "ToolBoxGui.h"
#include <QApplication>

#include "ToolBox.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GSNImageToolBox::ToolBox::InitializeMagickEnvironment();

    ToolBoxGui w;
    w.show();

    return a.exec();
}
