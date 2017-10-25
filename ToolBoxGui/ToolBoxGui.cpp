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
    initExplorer();
    initGallery();
    initMetadata();
    initImageToolbox();

    initConnections();
    initLayout();
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

    m_treeViewFolderExplorer->setModel(m_modelFolderExplorer);
    m_treeViewFolderExplorer->setColumnHidden(1, true);
    m_treeViewFolderExplorer->setColumnHidden(2, true);
    m_treeViewFolderExplorer->setColumnHidden(3, true);
    m_treeViewFolderExplorer->setRootIndex(m_modelFolderExplorer->setRootPath("C:/"));
    m_treeViewFolderExplorer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
}

void ToolBoxGui::initGallery()
{
    m_listGallery = new QListWidget(this);
    m_listGallery->setViewMode(QListWidget::IconMode);
    m_listGallery->setIconSize(QSize(120, 120));
    m_listGallery->setResizeMode(QListView::Adjust);
    m_listGallery->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_listGallery->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void ToolBoxGui::initMetadata()
{
    m_modelMetadata = new JsonModel(this);


    m_treeViewMetadata = new QTreeView(this);
    m_treeViewMetadata->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_treeViewMetadata->setModel(m_modelMetadata);
    m_treeViewMetadata->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ToolBoxGui::initImageToolbox()
{
    m_imageToolbox = new ImageManipulationToolbox(this);
}

void ToolBoxGui::initConnections()
{
    connect(m_browseButton, &QPushButton::clicked, m_browseDialog, &QFileDialog::exec);
    connect(m_browseDialog, &QFileDialog::fileSelected, [=](const QString &file)
                                                        { if (QDir(file).exists())
                                                              m_treeViewFolderExplorer->setRootIndex(m_modelFolderExplorer->setRootPath(file));
                                                          else
                                                              qWarning() << "Invalid directory selected";
                                                        });

    connect(m_treeViewFolderExplorer, &QTreeView::doubleClicked, this, &ToolBoxGui::slotGalleryDirectoryChanged);

    connect(m_listGallery, &QListWidget::clicked, this, &ToolBoxGui::slotCurrentPictureChanged);
    connect(m_listGallery, &QListWidget::itemSelectionChanged, this, &ToolBoxGui::slotGallerySelectionChanged);
}

void ToolBoxGui::initLayout()
{
    m_layout = new QGridLayout(this);
    setLayout(m_layout);

    m_layout->addWidget(m_treeViewFolderExplorer, 1, 0);
    m_layout->addWidget(m_browseButton, 0, 0);
    m_layout->addWidget(m_listGallery, 1, 2);
    m_layout->addWidget(m_treeViewMetadata, 1, 3);
    m_layout->addWidget(m_imageToolbox, 0, 3);
}

void ToolBoxGui::slotGalleryDirectoryChanged(const QModelIndex &index)
{
    QString selectedPath = m_modelFolderExplorer->fileInfo(index).absoluteFilePath();
    if (selectedPath == m_currentGalleryPath)
        return;

    m_currentGalleryPath = selectedPath;
    m_currentGalleryListGUID = QUuid::createUuid().toString();

    /////////// gallery part START ///////////
    QDir currentGalleryDir(selectedPath);
    currentGalleryDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    currentGalleryDir.setNameFilters(GSNImageToolBox::common::Utils::filterImagesList);

    // maybe clean references to metadata part here as well
    QThreadPool::globalInstance()->clear();
    m_listGallery->clear();
    qDebug() << "Inserting " << currentGalleryDir.entryInfoList().size();
    for (QFileInfo fileInfo : currentGalleryDir.entryInfoList())
    {
        // move thumbnail creation to separate thread
        // set placeholder icon and exchange it with real image later
        QListWidgetItem* item = new QListWidgetItem(m_utilsGui.getIcon(GSNImageToolBox::common::EImageFormatFromString(fileInfo.suffix())), fileInfo.fileName(), m_listGallery);
        item->setSizeHint(QSize(m_listGallery->iconSize().width(), m_listGallery->iconSize().height() + (3 * item->font().pointSize())));
        ThumbnailLoader* loader = new ThumbnailLoader(m_listGallery->iconSize(), fileInfo.absoluteFilePath(), item->listWidget()->row(item), m_currentGalleryListGUID);
        connect(loader, &ThumbnailLoader::signalLoaded, this, &ToolBoxGui::slotThumbLoaded);
        QThreadPool::globalInstance()->start(loader);
    }
    /////////// gallery part END ///////////

    /////////// metadata part START ///////////

    /////////// metadata part END ///////////

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

void ToolBoxGui::slotCurrentPictureChanged(const QModelIndex &index)
{
    if (m_listGallery->selectedItems().size() > 1)
        return;

    QListWidgetItem* item = m_listGallery->item(index.row());
    if (item == Q_NULLPTR)
        return;
    QDir currentDir(m_currentGalleryPath);
    QString filePath = currentDir.absoluteFilePath(item->text());
    if (!currentDir.exists(item->text()))
    {
        qWarning() << filePath << "does not actually exist. Thats almost impossible";
        qWarning() << "but it might be that physical file was deleted";
        return;
    }
    GSNImageToolBox::ToolBox toolbox;
    m_modelMetadata->loadJson(toolbox.pingSource(filePath));
    m_treeViewMetadata->expandAll();
    m_treeViewMetadata->resizeColumnToContents(0);
}

void ToolBoxGui::slotGallerySelectionChanged()
{
    if (m_listGallery->selectedItems().size() > 1)
        m_modelMetadata->clear();
}
