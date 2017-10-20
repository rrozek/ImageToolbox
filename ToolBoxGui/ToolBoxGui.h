#ifndef TOOLBOXGUI_H
#define TOOLBOXGUI_H

#include <QDialog>

#include <QFileSystemModel>
#include <QTreeView>
#include <QGridLayout>
#include <QListWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QListView>

#include "model/JsonModel.h"

class ToolBoxGui : public QDialog
{
    Q_OBJECT

public:
    ToolBoxGui(QWidget *parent = 0);
    ~ToolBoxGui();

private:
    void initExplorer();
    void initGallery();
    void initMetadata();

    QFileSystemModel *m_modelFolderExplorer;
    QFileSystemModel *m_modelGallery;
    JsonModel *m_modelMetadata;


    QPushButton *m_browseButton;
    QFileDialog *m_browseDialog;

    QTreeView *m_treeViewFolderExplorer;
    QTreeView *m_treeViewMetadata;
//    QListWidget* m_listGallery;
    QListView *m_listGallery;


    QGridLayout *m_layout;
};

#endif // TOOLBOXGUI_H
