#include "ThumbnailLoader.h"

#include "ToolBox.h"

ThumbnailLoader::ThumbnailLoader(const QSize &thumbSize, const QFileInfo &file, int rowOfItemToUpdate, const QString& listGUID )
    : m_thumbSize(thumbSize)
    , m_fileInfo(file)
    , m_rowOfItemToUpdate(rowOfItemToUpdate)
    , m_listGUID(listGUID)
{
}

void ThumbnailLoader::run()
{
    GSNImageToolBox::ToolBox toolbox;

    toolbox.setSource(m_fileInfo.absoluteFilePath());
    QByteArray thumbnailData;
    toolbox.getThumbnail(static_cast<quint32>(m_thumbSize.width()), static_cast<quint32>(m_thumbSize.height()), GSNImageToolBox::common::PNG, thumbnailData);
    QPixmap iconPixmap(m_thumbSize);
    iconPixmap.loadFromData(thumbnailData);
    QIcon icon(iconPixmap);
    emit signalLoaded(icon, m_rowOfItemToUpdate, m_listGUID);
}
