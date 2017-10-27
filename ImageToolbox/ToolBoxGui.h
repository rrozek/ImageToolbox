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
#include "ImageManipulationToolbox.h"
#include "UtilsGUI.h"
#include "WaitingSpinnerWidget.h"

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
    void initImageToolbox();
    void initConnections();
    void initLayout();

    void slotGalleryDirectoryChanged(const QModelIndex& index);
    void slotAdjustExplorerToContents(const QModelIndex& index);

    void slotThumbLoaded(QIcon icon, int rowOfItemToUpdate, QString listGUID);
    void slotMetadataLoaded(QJsonDocument doc, QString imageGUID);
    void slotCurrentPictureChanged(const QModelIndex &index);
    void slotGallerySelectionChanged();

    void slotToolboxBusy();
    void slotToolboxReady();

    QFileSystemModel *m_modelFolderExplorer;
    JsonModel *m_modelMetadata;


    QPushButton *m_browseButton;
    QFileDialog *m_browseDialog;

    QTreeView *m_treeViewFolderExplorer;
    QTreeView *m_treeViewMetadata;
    WaitingSpinnerWidget* m_waitingSpinnerMetadata;
    WaitingSpinnerWidget* m_waitingSpinnerToolbox;
    QListWidget* m_listGallery;

    ImageManipulationToolbox* m_imageToolbox;

    QString m_currentGalleryListGUID;
    QString m_currentImageGUID;
    QString m_currentGalleryPath;

    GSNImageToolBox::common::UtilsGUI m_utilsGui;

    QGridLayout *m_layout;
};

