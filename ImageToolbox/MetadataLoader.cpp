#include "MetadataLoader.h"

#include "ToolBox.h"

MetadataLoader::MetadataLoader(const QString &filePath, const QString& imgGUID)
    : m_filePath(filePath)
    , m_imgGUID(imgGUID)
{
}

void MetadataLoader::run()
{
    GSNImageToolBox::ToolBox toolbox;
    QJsonDocument doc = toolbox.pingSource(m_filePath);

    emit signalLoaded(doc, m_imgGUID);
}
