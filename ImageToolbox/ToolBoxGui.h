#pragma once

#include <QDialog>

#include <QFileSystemModel>
#include <QTreeView>
#include <QGridLayout>
#include <QListWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QListView>

#include "model/JsonModel.h"
#include "ToolBox.h"
#include "ImageManipulationToolbox.h"
#include "UtilsGUI.h"
#include "WaitingSpinnerWidget.h"
#include "ConfigDialog.h"

class ToolBoxGui : public QDialog
{
    Q_OBJECT

public:
    ToolBoxGui(QString startDirectory = QString(), QWidget *parent = 0);
    ~ToolBoxGui();

private:
    void initMenu();
    void initConfiguration();
    void initExplorer(const QString &startDirectory);
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

    void slotFolderExplorerContextMenuRequested(const QPoint& pos);
    void slotGalleryContextMenuRequested(const QPoint& pos);

    void slotToolboxBusy();
    void slotToolboxReady();

    void slotUpdateSettings();

    QStringList getFileNameFilters() const;

    QFileSystemModel *m_modelFolderExplorer;
    JsonModel *m_modelMetadata;

    QMenuBar *m_menuBar;
    QMenu *m_menuConfiguration;
    QAction *m_actionConfiguration;

    QTreeView *m_treeViewFolderExplorer;
    QTreeView *m_treeViewMetadata;
    WaitingSpinnerWidget* m_waitingSpinnerMetadata;
    WaitingSpinnerWidget* m_waitingSpinnerToolbox;
    QListWidget* m_listGallery;

    ImageManipulationToolbox* m_imageToolbox;

    ConfigDialog* m_configDialog;
    std::shared_ptr<QSettings> m_settings;

    QString m_currentGalleryListGUID;
    QString m_currentImageGUID;
    QString m_currentGalleryPath;

    GSNImageToolBox::common::UtilsGUI m_utilsGui;

    QGridLayout *m_layout;
};

