#include "ToolBoxGui.h"

#include <QDebug>
#include <QJsonObject>


#include "Utils.h"

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

//    m_modelGallery = new QFileSystemModel(this);
//    m_modelGallery->setFilter(QDir::NoDotAndDotDot | QDir::Files);
//    m_modelGallery->setNameFilters(common::Utils::filterImagesList);
//    m_modelGallery->setNameFilterDisables(false);

//    m_listGallery->setModel(m_modelGallery);
//    m_listGallery->setRootIndex(m_modelGallery->setRootPath("C:/"));


    m_layout->addWidget(m_listGallery, 1, 2);
}

void ToolBoxGui::initMetadata()
{
    m_treeViewMetadata = new QTreeView(this);
    m_layout->addWidget(m_treeViewMetadata, 1, 3);
}

void ToolBoxGui::slotGalleryDirectoryChanged(const QModelIndex &index)
{
    QString galleryPath = m_modelFolderExplorer->fileInfo(index).absoluteDir().absolutePath();
    QDir currentGalleryDir(m_modelFolderExplorer->fileInfo(index).absoluteFilePath());
    currentGalleryDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    currentGalleryDir.setNameFilters(common::Utils::filterImagesList);

    // maybe clean references to metadata part here as well
    m_listGallery->clear();
    qDebug() << "Inserting " << currentGalleryDir.entryInfoList().size();
    for (QFileInfo fileInfo : currentGalleryDir.entryInfoList())
    {
        // create Icon using thumbnail from ImageToolboxLib here
        m_toolbox.setSource(fileInfo.absoluteFilePath());
        QJsonObject metaData;
        m_toolbox.getImageInfo().dumpTo(metaData);
        double percentCrop = calculatePercentCrop(QSize(metaData.value("root[0].image.geometry.width").toInt(), metaData.value("root[0].image.geometry.height").toInt())
                                                 , m_listGallery->iconSize());
        qDebug() << "percentCrop " << percentCrop;
        QByteArray thumbnailData;
        m_toolbox.getThumbnail(percentCrop, thumbnailData);
        QPixmap iconPixmap(m_listGallery->iconSize());
        iconPixmap.loadFromData(thumbnailData);
        QIcon icon(iconPixmap);
        QListWidgetItem* item = new QListWidgetItem(fileInfo.fileName(), m_listGallery);
        Q_UNUSED(item)
    }
}

double ToolBoxGui::calculatePercentCrop(const QSize &source, const QSize &destination)
{
    bool cropNeeded = false;
    if (source.width() > destination.width())
        cropNeeded = true;
    if (source.height() > destination.height())
        cropNeeded = true;

    if (cropNeeded)
        return 1.0 / qMax(static_cast<double>(source.width()) / static_cast<double>(destination.width())
                         , static_cast<double>(source.height()) / static_cast<double>(destination.height())
                         );
    return 1.0;
}
