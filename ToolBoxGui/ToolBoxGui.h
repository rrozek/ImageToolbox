#ifndef TOOLBOXGUI_H
#define TOOLBOXGUI_H

#include <QDialog>

class ToolBoxGui : public QDialog
{
    Q_OBJECT

public:
    ToolBoxGui(QWidget *parent = 0);
    ~ToolBoxGui();
};

#endif // TOOLBOXGUI_H
