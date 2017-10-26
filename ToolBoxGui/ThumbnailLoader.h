#pragma once

#include <QObject>
#include <QRunnable>
#include <QFileInfo>
#include <QSize>
#include <QString>
#include <QIcon>

class ThumbnailLoader : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ThumbnailLoader(const QSize& thumbSize, const QFileInfo &file, int rowOfItemToUpdate , const QString &listGUID);
    void run() override;

signals:
    void signalLoaded(QIcon icon, int rowOfItemToUpdate, QString listGUID);
private:
    QSize m_thumbSize;
    QFileInfo m_fileInfo;
    int m_rowOfItemToUpdate;
    QString m_listGUID;
};

