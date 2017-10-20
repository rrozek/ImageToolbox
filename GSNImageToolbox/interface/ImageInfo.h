#pragma once

#include <QSize>
#include <QList>
#include <QVector>
#include <QObject>


#include "Common.h"
#include <memory>

#include <QJsonDocument>

class JsonObjectRoot;

namespace Magick
{
class Image;
}

namespace GSNImageToolBox
{
class ImageInfo
{
public:
    explicit ImageInfo();
    ~ImageInfo();

    bool loadJson(const QByteArray& json);
    bool loadJson(const QJsonDocument& jsonDoc);

    bool dumpTo(QJsonObject &jsonObject) const;
    bool dumpTo(QJsonArray &jsonArray) const;
    bool dumpTo(QJsonDocument &jsonDocument) const;
    bool dumpTo(QByteArray &byteArray) const;

    QStringList getAvailableProperties(const QString pattern = QString::null) const;
    QVariant getValue(const QString& propertyKey) const;
    bool setValue(const QString& key, const QVariant& value);

    void print(int imageNumber = -1) const;

//    bool isContainer() const;
//    quint8 getImagesCount() const;
//    const ImageInfo& getImageInfo(quint8 imageNumber = 0) const;

    static ImageInfo invalid;
private:
    QJsonValue dump() const;

    JsonObjectRoot* m_rootItem;
    QJsonDocument m_document;

};

} // namespace GSNImageToolBox


