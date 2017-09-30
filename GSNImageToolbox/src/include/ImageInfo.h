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
class ImageInfo : public QObject
{
public:
    explicit ImageInfo(QObject* parent = Q_NULLPTR);
    ~ImageInfo();

    bool loadJson(const QByteArray& json);
    bool loadJson(const QJsonDocument& jsonDoc);

    bool dumpTo(QJsonObject &jsonObject) const;
    bool dumpTo(QJsonArray &jsonArray) const;
    bool dumpTo(QJsonDocument &jsonDocument) const;
    bool dumpTo(QByteArray &byteArray) const;

    void print(int imageNumber = -1) const;

//    bool isContainer() const;
//    quint8 getImagesCount() const;
//    const ImageInfo& getImageInfo(quint8 imageNumber = 0) const;

private:
    QJsonValue dump() const;

    JsonObjectRoot* m_rootItem;
    QJsonDocument m_document;

};

} // namespace GSNImageToolBox


