#include "ToolBoxGui.h"

#include <QDebug>

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
    m_modelFolderExplorer->setReadOnly(true);

    m_browseButton = new QPushButton(tr("Select base directory"), this);
    m_browseDialog = new QFileDialog(this);
    m_browseDialog->setFileMode(QFileDialog::Directory);

    m_treeViewFolderExplorer = new QTreeView(this);

    connect(m_browseButton, &QPushButton::clicked, m_browseDialog, &QFileDialog::exec);
    connect(m_browseDialog, &QFileDialog::fileSelected, [=](const QString &file)
                                                        { m_treeViewFolderExplorer->setRootIndex(m_modelFolderExplorer->setRootPath(file)); });

    m_treeViewFolderExplorer->setModel(m_modelFolderExplorer);
    m_treeViewFolderExplorer->setColumnHidden(1, true);
    m_treeViewFolderExplorer->setColumnHidden(2, true);
    m_treeViewFolderExplorer->setColumnHidden(3, true);

    connect(m_treeViewFolderExplorer, &QTreeView::doubleClicked, [=](const QModelIndex& index)
                                                                 { QString galleryPath = m_modelFolderExplorer->fileInfo(index).absoluteFilePath();
                                                                   m_listGallery->setRootIndex(m_modelGallery->setRootPath(galleryPath));
                                                                 });

    m_layout->addWidget(m_treeViewFolderExplorer, 1, 0);
    m_layout->addWidget(m_browseButton, 0, 0);
}

void ToolBoxGui::initGallery()
{
    m_listGallery = new QListView(this);
    m_modelGallery = new QFileSystemModel(this);
    m_listGallery->setModel(m_modelGallery);
    m_listGallery->setViewMode(QListView::IconMode);
    m_listGallery->setIconSize(QSize(200, 200));
    m_listGallery->setResizeMode(QListView::Adjust);

    m_layout->addWidget(m_listGallery, 1, 2);
}

void ToolBoxGui::initMetadata()
{
    m_treeViewMetadata = new QTreeView(this);
    m_layout->addWidget(m_treeViewMetadata, 1, 3);
}
