#include "ImageInfo.h"

#include <QDebug>
#include "Magick++.h"

#include "src/impl/jsonObjects/JsonObjectRoot.h"

namespace GSNImageToolBox
{

ImageInfo::ImageInfo(QObject *parent)
    : m_rootItem(new JsonObjectRoot(Q_NULLPTR))
{
}

ImageInfo::~ImageInfo()
{
}

//! Loads JSON from \a json bytearray into model
bool ImageInfo::loadJson(const QByteArray &json)
{
    return loadJson(QJsonDocument::fromJson(json));
}

//! Loads JSON from \a jsonDoc document into model
bool ImageInfo::loadJson(const QJsonDocument &jsonDoc)
{
    m_document = QJsonDocument(jsonDoc);
    if (!m_document.isNull())
    {
        if (m_rootItem != Q_NULLPTR)
            delete m_rootItem;
        m_rootItem = new JsonObjectRoot(Q_NULLPTR);
        if (m_document.isArray()) {
            m_rootItem->deserialize(QJsonValue(m_document.array()));
            m_rootItem->setType(QJsonValue::Array);
        } else {
            m_rootItem->deserialize(QJsonValue(m_document.object()));
            m_rootItem->setType(QJsonValue::Object);
        }
        return true;
    }

    qDebug()<< "cannot load json";
    return false;
}

//! Dumps model data into \a jsonObject
bool ImageInfo::dumpTo(QJsonObject &jsonObject) const
{
    jsonObject = dump().toObject();
    return true;
}

//! Dumps model data into \a jsonArray
bool ImageInfo::dumpTo(QJsonArray &jsonArray) const
{
    jsonArray = dump().toArray();
    return true;
}

//! Dumps model data into \a jsonDocument
bool ImageInfo::dumpTo(QJsonDocument &jsonDocument) const
{
    jsonDocument = QJsonDocument(dump().toObject());
    return true;
}

//! Dumps model data into \a byteArray
bool ImageInfo::dumpTo(QByteArray &byteArray) const
{
    byteArray.clear();
    QJsonDocument doc;
    if (dumpTo(doc))
    {
        byteArray = doc.toBinaryData();
        return true;
    }
    return false;
}

QStringList ImageInfo::getAvailableProperties(const QString &regexPattern) const
{
    QStringList properties;
    return properties;
}

QVariant ImageInfo::getValue(const QString &propertyKey) const
{
    JsonObjectBase* obj = m_rootItem->getObjectByPath(propertyKey);
    if (obj == Q_NULLPTR)
        return QVariant();
    qDebug() << "key value pair: " << obj->key() << obj->value().toVariant();
    return obj->value().toVariant();
}

bool ImageInfo::setValue(const QString &key, const QVariant &value)
{

    return true;
}

//! Dumps model data into new QJsonValue
QJsonValue ImageInfo::dump() const
{
    qDebug() << Q_FUNC_INFO;
    return m_rootItem->serialize();
}

void ImageInfo::print(int /*imageNumber*/) const
{
    qDebug() << Q_FUNC_INFO;
    m_rootItem->printPaths();
}

} // namespace GSNImageToolBox
