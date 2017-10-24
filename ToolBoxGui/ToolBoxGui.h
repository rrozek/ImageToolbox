#pragma once

#include <QDialog>

#include <QFileSystemModel>
#include <QTreeView>
#include <QGridLayout>
#include <QListWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QListView>

#include "model/JsonModel.h"
#include "ToolBox.h"
#include "UtilsGUI.h"

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

    void slotGalleryDirectoryChanged(const QModelIndex& index);

    void slotThumbLoaded(QIcon icon, int rowOfItemToUpdate, QString listGUID);

    QFileSystemModel *m_modelFolderExplorer;
    JsonModel *m_modelMetadata;


    QPushButton *m_browseButton;
    QFileDialog *m_browseDialog;

    QTreeView *m_treeViewFolderExplorer;
    QTreeView *m_treeViewMetadata;
    QListWidget* m_listGallery;
    QString m_currentGalleryListGUID;

    GSNImageToolBox::common::UtilsGUI m_utilsGui;

    QGridLayout *m_layout;
};

