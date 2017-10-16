#include "ToolBoxGui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToolBoxGui w;
    w.show();

    return a.exec();
}
