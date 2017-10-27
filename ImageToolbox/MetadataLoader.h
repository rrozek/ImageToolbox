#pragma once

#include <QObject>
#include <QRunnable>
#include <QFileInfo>
#include <QSize>
#include <QString>

#include <QJsonDocument>

class MetadataLoader : public QObject, public QRunnable
{
    Q_OBJECT
public:
    MetadataLoader(const QString &filePath, const QString& imgGUID);
    void run() override;

signals:
    void signalLoaded(QJsonDocument doc, QString imgGUID);
private:
    QString m_filePath;
    QString m_imgGUID;
};

