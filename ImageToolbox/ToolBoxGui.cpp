#include "ToolBoxGui.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>
#include <QThreadPool>
#include <QUuid>
#include <QProcess>
#include <QDesktopServices>
#include <QApplication>

#include "ThumbnailLoader.h"
#include "MetadataLoader.h"
#include "Utils.h"
#include "UtilsGUI.h"

ToolBoxGui::ToolBoxGui(QString startDirectory, QWidget *parent)
    : QDialog(parent)
    , m_waitingSpinnerMetadata(Q_NULLPTR)
    , m_waitingSpinnerToolbox(Q_NULLPTR)
{
    m_settings = std::make_shared<QSettings>("settings.ini", QSettings::IniFormat);

    initConfiguration();
    initMenu();
    initExplorer(startDirectory);
    initGallery();
    initMetadata();
    initImageToolbox();


    initConnections();
    initLayout();

    setMinimumSize(800, 500);
}

ToolBoxGui::~ToolBoxGui()
{
}

void ToolBoxGui::initMenu()
{
    m_menuBar = new QMenuBar(this);
    m_menuConfiguration = m_menuBar->addMenu(tr("Menu"));
    m_menuConfiguration->addAction(tr("Configuration"), m_configDialog, &ConfigDialog::show);
}

void ToolBoxGui::initConfiguration()
{
    m_configDialog = new ConfigDialog(m_settings, this);
    m_configDialog->setModal(true);
}

void ToolBoxGui::initExplorer(const QString& startDirectory)
{
    m_modelFolderExplorer = new QFileSystemModel(this);
    m_modelFolderExplorer->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    m_modelFolderExplorer->setRootPath("C:/");

    m_treeViewFolderExplorer = new QTreeView(this);

    m_treeViewFolderExplorer->setModel(m_modelFolderExplorer);
    m_treeViewFolderExplorer->hideColumn(1);
    m_treeViewFolderExplorer->hideColumn(2);
    m_treeViewFolderExplorer->hideColumn(3);
    m_treeViewFolderExplorer->setMinimumWidth(100);

    if (!startDirectory.isEmpty())
    {
        m_treeViewFolderExplorer->setCurrentIndex(m_modelFolderExplorer->index(startDirectory));
    }
    else if (QDir(m_settings->value(GSNImageToolBox::common::dirBaseDir).toString()).exists())
        m_treeViewFolderExplorer->setCurrentIndex(m_modelFolderExplorer->
                                               index(m_settings->value(GSNImageToolBox::common::dirBaseDir).toString()));
    else
        qWarning() << "Invalid directory selected";

    m_treeViewFolderExplorer->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeViewFolderExplorer, &QTreeView::customContextMenuRequested, this, &ToolBoxGui::slotFolderExplorerContextMenuRequested);

    m_treeViewFolderExplorer->resizeColumnToContents(0);
}

void ToolBoxGui::initGallery()
{
    m_listGallery = new QListWidget(this);
    m_listGallery->setViewMode(QListWidget::IconMode);
    m_listGallery->setIconSize(QSize(120, 120));
    m_listGallery->setResizeMode(QListView::Adjust);
//    m_listGallery->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_listGallery->setMinimumWidth(500);
    m_listGallery->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_listGallery->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_listGallery, &QTreeView::customContextMenuRequested, this, &ToolBoxGui::slotGalleryContextMenuRequested);
}

void ToolBoxGui::initMetadata()
{
    m_modelMetadata = new JsonModel(this);


    m_treeViewMetadata = new QTreeView(this);
    m_treeViewMetadata->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_treeViewMetadata->setModel(m_modelMetadata);
    m_treeViewMetadata->setMinimumWidth(100);
//    m_treeViewMetadata->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
}

void ToolBoxGui::initImageToolbox()
{
    m_imageToolbox = new ImageManipulationToolbox(this);
    m_imageToolbox->setSettings(m_settings);
    connect(m_imageToolbox, &ImageManipulationToolbox::signalBusy, this, &ToolBoxGui::slotToolboxBusy);
    connect(m_imageToolbox, &ImageManipulationToolbox::signalReady, this, &ToolBoxGui::slotToolboxReady);
}

void ToolBoxGui::initConnections()
{
    connect(m_treeViewFolderExplorer, &QTreeView::doubleClicked, this, &ToolBoxGui::slotGalleryDirectoryChanged);
    connect(m_treeViewFolderExplorer, &QTreeView::clicked, this, &ToolBoxGui::slotAdjustExplorerToContents);

    connect(m_listGallery, &QListWidget::clicked, this, &ToolBoxGui::slotCurrentPictureChanged);
    connect(m_listGallery, &QListWidget::itemSelectionChanged, this, &ToolBoxGui::slotGallerySelectionChanged);

    connect(m_configDialog, &ConfigDialog::signalSettingsChanged, this, &ToolBoxGui::slotUpdateSettings);

}

void ToolBoxGui::initLayout()
{
    m_layout = new QGridLayout(this);
    setLayout(m_layout);

    m_layout->setMenuBar(m_menuBar);
    m_layout->addWidget(m_treeViewFolderExplorer, 0, 0, 2, 1);
    m_layout->addWidget(m_listGallery, 0, 1, 2, 1);
    m_layout->addWidget(m_imageToolbox, 0, 2);
    m_layout->addWidget(m_treeViewMetadata, 1, 2);
}

void ToolBoxGui::slotGalleryDirectoryChanged(const QModelIndex &index)
{
    m_treeViewFolderExplorer->resizeColumnToContents(0);
    QString selectedPath = m_modelFolderExplorer->fileInfo(index).absoluteFilePath();

    m_currentGalleryPath = selectedPath;
    m_currentGalleryListGUID = QUuid::createUuid().toString();

    QDir currentGalleryDir(selectedPath);
    currentGalleryDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    currentGalleryDir.setNameFilters(getFileNameFilters());

    QThreadPool::globalInstance()->clear();
    m_listGallery->clear();
    for (QFileInfo fileInfo : currentGalleryDir.entryInfoList())
    {
        QListWidgetItem* item = new QListWidgetItem(m_utilsGui.getIcon(GSNImageToolBox::common::EImageFormatFromString(fileInfo.suffix())), fileInfo.fileName(), m_listGallery);
        item->setSizeHint(QSize(m_listGallery->iconSize().width(), m_listGallery->iconSize().height() + (3 * item->font().pointSize())));
        ThumbnailLoader* loader = new ThumbnailLoader(m_listGallery->iconSize(), fileInfo, item->listWidget()->row(item), m_currentGalleryListGUID);
        connect(loader, &ThumbnailLoader::signalLoaded, this, &ToolBoxGui::slotThumbLoaded);
        QThreadPool::globalInstance()->start(loader);
    }
}

void ToolBoxGui::slotAdjustExplorerToContents(const QModelIndex &/*index*/)
{
    m_treeViewFolderExplorer->resizeColumnToContents(0);
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

void ToolBoxGui::slotMetadataLoaded(QJsonDocument doc, QString imageGUID)
{
    if (m_currentImageGUID != imageGUID)
    {
        qWarning() << "list already invalid";
        return;
    }

    m_modelMetadata->loadJson(doc);

    m_waitingSpinnerMetadata->stop();
    m_treeViewMetadata->expandAll();
    m_treeViewMetadata->resizeColumnToContents(0);
}

void ToolBoxGui::slotCurrentPictureChanged(const QModelIndex &index)
{
    if (m_listGallery->selectedItems().size() != 1)
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

    m_currentImageGUID = QUuid::createUuid().toString();

    if (m_waitingSpinnerMetadata != Q_NULLPTR)
        delete m_waitingSpinnerMetadata;
    m_waitingSpinnerMetadata = new WaitingSpinnerWidget(m_treeViewMetadata);
    m_waitingSpinnerMetadata->start();

    MetadataLoader* loader = new MetadataLoader(filePath, m_currentImageGUID);
    connect(loader, &MetadataLoader::signalLoaded, this, &ToolBoxGui::slotMetadataLoaded);
    QThreadPool::globalInstance()->start(loader);

    m_imageToolbox->slotUpdateViewToFile(QFileInfo(filePath));
}

void ToolBoxGui::slotGallerySelectionChanged()
{
    if (m_listGallery->selectedItems().size() != 1)
    {
        m_modelMetadata->clear();
        QFileInfoList listOfFiles;
        for (QListWidgetItem* item : m_listGallery->selectedItems())
        {
            QDir currentDir(m_currentGalleryPath);
            QString filePath = currentDir.absoluteFilePath(item->text());
            if (!currentDir.exists(item->text()))
            {
                qWarning() << filePath << "does not actually exist. Thats almost impossible";
                qWarning() << "but it might be that physical file was deleted";
                continue;
            }
            listOfFiles.append(QFileInfo(filePath));
        }
        m_imageToolbox->slotUpdateViewToFile(listOfFiles);
    }
}

void ToolBoxGui::slotFolderExplorerContextMenuRequested(const QPoint &pos)
{
    QModelIndex hoveredIndex = m_treeViewFolderExplorer->indexAt(pos);
    if (!hoveredIndex.isValid())
        return;

    QMenu *menu = new QMenu(this);
    menu->addAction(tr("Show in Explorer"), this, [=]
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_modelFolderExplorer->filePath(hoveredIndex)));
    });
    menu->addAction(tr("Open in new instance"), this, [=]
    {
        QProcess::startDetached(QApplication::applicationFilePath(), QStringList({m_modelFolderExplorer->filePath(hoveredIndex)}));
    });

    menu->popup(m_treeViewFolderExplorer->viewport()->mapToGlobal(pos));
}

void ToolBoxGui::slotGalleryContextMenuRequested(const QPoint &pos)
{
    QModelIndex hoveredIndex = m_listGallery->indexAt(pos);
    if (!hoveredIndex.isValid())
        return;

    QDir currentDir(m_currentGalleryPath);
    QString filePath = currentDir.absoluteFilePath(hoveredIndex.data(Qt::DisplayRole).toString());
    if (!currentDir.exists(hoveredIndex.data(Qt::DisplayRole).toString()))
    {
        qWarning() << filePath << "does not actually exist. Thats almost impossible";
        qWarning() << "but it might be that physical file was deleted";
        return;
    }
    QFileInfo fileInfo(filePath);
    GSNImageToolBox::common::EImageFormat imageFormat = GSNImageToolBox::common::EImageFormatFromString(fileInfo.completeSuffix());

    QMenu *menu = new QMenu(this);
    menu->addAction(tr("Open"), this, [=]
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));;
    });

    QMenu *convertMenu = menu->addMenu(tr("Convert"));
    QAction *actionToJpeg = convertMenu->addAction(tr("to JPEG"), this, [=]
    {
        m_imageToolbox->scheduleFileConversion(fileInfo, GSNImageToolBox::common::JPEG);
    });
    QAction *actionToPng = convertMenu->addAction(tr("to PNG"), this, [=]
    {
        m_imageToolbox->scheduleFileConversion(fileInfo, GSNImageToolBox::common::PNG);
    });
    QAction *actionToTiff = convertMenu->addAction(tr("to TIFF"), this, [=]
    {
        m_imageToolbox->scheduleFileConversion(fileInfo, GSNImageToolBox::common::TIFF);
    });

    actionToTiff->setEnabled(false);
    actionToJpeg->setEnabled(false);
    actionToPng->setEnabled(false);
    if (imageFormat == GSNImageToolBox::common::TIFF)
    {
        actionToJpeg->setEnabled(true);
        actionToPng->setEnabled(true);
    }
    if (imageFormat == GSNImageToolBox::common::PNG)
    {
        actionToJpeg->setEnabled(true);
    }
    if (imageFormat == GSNImageToolBox::common::JPEG)
    {
        actionToPng->setEnabled(true);
    }
    if (imageFormat == GSNImageToolBox::common::EPS)
    {
        actionToTiff->setEnabled(true);
        actionToJpeg->setEnabled(true);
        actionToPng->setEnabled(true);
    }

    // now to comply with settings
    m_settings->beginGroup("SupportedFormats");
    if (!m_settings->value("TIF").toBool())
        actionToTiff->setEnabled(false);
    if (!m_settings->value("JPEG").toBool())
        actionToJpeg->setEnabled(false);
    if (!m_settings->value("PNG").toBool())
        actionToPng->setEnabled(false);
    m_settings->endGroup();


    menu->addSeparator();
    menu->addAction(tr("Delete"), this, [=]
    {
        if (QFile::remove(filePath))
        {
            QListWidgetItem* removedItem = m_listGallery->takeItem(hoveredIndex.row());
            delete removedItem;
        }
    });

    menu->popup(m_listGallery->viewport()->mapToGlobal(pos));
}

void ToolBoxGui::slotToolboxBusy()
{
    if (m_waitingSpinnerToolbox != Q_NULLPTR)
        delete m_waitingSpinnerToolbox;
    m_waitingSpinnerToolbox = new WaitingSpinnerWidget(this);
    m_waitingSpinnerToolbox->start();
}

void ToolBoxGui::slotToolboxReady()
{
    m_waitingSpinnerToolbox->stop();
}

void ToolBoxGui::slotUpdateSettings()
{
    // updating view explorer
    if (QDir(m_settings->value(GSNImageToolBox::common::dirBaseDir).toString()).exists())
        m_treeViewFolderExplorer->setCurrentIndex(m_modelFolderExplorer->
                                               index(m_settings->value(GSNImageToolBox::common::dirBaseDir).toString()));
    else
        qWarning() << "Invalid directory selected";

    // updating file filters
    slotGalleryDirectoryChanged(m_treeViewFolderExplorer->currentIndex());
    m_listGallery->clearSelection();

    // updating conversion toolbox is done on the fly

}

QStringList ToolBoxGui::getFileNameFilters() const
{
    QStringList result;
    result.append("*.someBullshitToFilterAny");
    m_settings->beginGroup("SupportedFormats");
    for (QString key : m_settings->allKeys())
    {
        if (m_settings->value(key).toBool())
        {
            result.append(QString("*." + QString(key).toUpper()));
            result.append(QString("*." + QString(key).toLower()));
            if (key == "JPEG")
            {
                result.append("*.jpg");
                result.append("*.JPG");
            }
            if (key == "TIF")
            {
                result.append("*.tiff");
                result.append("*.TIFF");
            }
        }
    }
    m_settings->endGroup();
    qDebug() << __func__ << result;
    return result;
}


