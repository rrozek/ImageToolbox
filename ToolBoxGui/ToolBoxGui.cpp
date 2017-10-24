#include "ToolBoxGui.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>
#include <QThreadPool>
#include <QUuid>

#include "ThumbnailLoader.h"
#include "Utils.h"
#include "UtilsGUI.h"

ToolBoxGui::ToolBoxGui(QWidget *parent)
    : QDialog(parent)
{

    m_layout = new QGridLayout(this);
    setLayout(m_layout);

    initExplorer();
    initGallery();
    initMetadata();
}

ToolBoxGui::~ToolBoxGui()
{

}

void ToolBoxGui::initExplorer()
{
    m_modelFolderExplorer = new QFileSystemModel(this);
    m_modelFolderExplorer->setFilter(QDir::NoDotAndDotDot| QDir::AllDirs);

    m_browseButton = new QPushButton(tr("Select base directory"), this);
    m_browseDialog = new QFileDialog(this);
    m_browseDialog->setFileMode(QFileDialog::Directory);

    m_treeViewFolderExplorer = new QTreeView(this);

    connect(m_browseButton, &QPushButton::clicked, m_browseDialog, &QFileDialog::exec);
    connect(m_browseDialog, &QFileDialog::fileSelected, [=](const QString &file)
                                                        { if (QDir(file).exists())
                                                              m_treeViewFolderExplorer->setRootIndex(m_modelFolderExplorer->setRootPath(file));
                                                          else
                                                              qWarning() << "Invalid directory selected";
                                                        });

    m_treeViewFolderExplorer->setModel(m_modelFolderExplorer);
    m_treeViewFolderExplorer->setColumnHidden(1, true);
    m_treeViewFolderExplorer->setColumnHidden(2, true);
    m_treeViewFolderExplorer->setColumnHidden(3, true);
    m_treeViewFolderExplorer->setRootIndex(m_modelFolderExplorer->setRootPath("C:/"));

    connect(m_treeViewFolderExplorer, &QTreeView::doubleClicked, this, &ToolBoxGui::slotGalleryDirectoryChanged);

    m_layout->addWidget(m_treeViewFolderExplorer, 1, 0);
    m_layout->addWidget(m_browseButton, 0, 0);
}

void ToolBoxGui::initGallery()
{
    m_listGallery = new QListWidget(this);
    m_listGallery->setViewMode(QListWidget::IconMode);
    m_listGallery->setIconSize(QSize(120, 120));
    m_listGallery->setResizeMode(QListView::Adjust);

    m_layout->addWidget(m_listGallery, 1, 2);
}

void ToolBoxGui::initMetadata()
{
    m_treeViewMetadata = new QTreeView(this);
    m_layout->addWidget(m_treeViewMetadata, 1, 3);
}

void ToolBoxGui::slotGalleryDirectoryChanged(const QModelIndex &index)
{
    m_currentGalleryListGUID = QUuid::createUuid().toString();
    QDir currentGalleryDir(m_modelFolderExplorer->fileInfo(index).absoluteFilePath());
    currentGalleryDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    currentGalleryDir.setNameFilters(GSNImageToolBox::common::Utils::filterImagesList);

    // maybe clean references to metadata part here as well
    m_listGallery->clear();
    qDebug() << "Inserting " << currentGalleryDir.entryInfoList().size();
    for (QFileInfo fileInfo : currentGalleryDir.entryInfoList())
    {
        // move thumbnail creation to separate thread
        // set placeholder icon and exchange it with real image later
        QListWidgetItem* item = new QListWidgetItem(m_utilsGui.getIcon(GSNImageToolBox::common::EImageFormatFromString(fileInfo.suffix())), fileInfo.fileName(), m_listGallery);

        ThumbnailLoader* loader = new ThumbnailLoader(m_listGallery->iconSize(), fileInfo.absoluteFilePath(), item->listWidget()->row(item), m_currentGalleryListGUID);
        connect(loader, &ThumbnailLoader::signalLoaded, this, &ToolBoxGui::slotThumbLoaded);
        QThreadPool::globalInstance()->start(loader);

    }
}

void ToolBoxGui::slotThumbLoaded(QIcon icon, int rowOfItemToUpdate, QString listGUID)
{
    if (m_currentGalleryListGUID != listGUID)
    {
        qWarning() << "list already invalid";
        return;
    }
    QListWidgetItem* item = m_listGallery->item(rowOfItemToUpdate);
    if (item == Q_NULLPTR)
    {
        qWarning() << "Invalid item at row: " << rowOfItemToUpdate << "no icon set";
        return;
    }

    item->setIcon(icon);
}
