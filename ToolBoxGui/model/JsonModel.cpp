#include <QFile>
#include <QFont>

#include "JsonModel.h"
#include "JsonObjectRoot.h"
//=========================================================================

//! Constructor
JsonModel::JsonModel(QObject *parent)
    : QAbstractItemModel(parent)
    , mRootItem(new JsonObjectRoot(Q_NULLPTR))
{
    mHeaders.append("key");
    mHeaders.append("value");
}

//! Destructor
JsonModel::~JsonModel()
{
    delete mRootItem;
}

//! Loads JSON from \a fileName into model
bool JsonModel::load(const QString &fileName)
{
    QFile file(fileName);
    bool success = false;
    if (file.open(QIODevice::ReadOnly)) {
        success = load(&file);
        file.close();
    }
    else success = false;

    return success;
}

//! Loads JSON from \a device into model
bool JsonModel::load(QIODevice *device)
{
    return loadJson(device->readAll());
}

//! Loads JSON from \a json bytearray into model
bool JsonModel::loadJson(const QByteArray &json)
{
    return loadJson(QJsonDocument::fromJson(json));
}

//! Loads JSON from \a jsonDoc document into model
bool JsonModel::loadJson(const QJsonDocument &jsonDoc)
{
    mDocument = jsonDoc;
    if (!mDocument.isNull())
    {
        beginResetModel();
        if (mRootItem != Q_NULLPTR)
            delete mRootItem;
        mRootItem = new JsonObjectRoot(Q_NULLPTR);
        if (mDocument.isArray()) {
            mRootItem->deserialize(QJsonValue(mDocument.array()));
            mRootItem->setType(QJsonValue::Array);
        } else {
            mRootItem->deserialize(QJsonValue(mDocument.object()));
            mRootItem->setType(QJsonValue::Object);
        }
        endResetModel();
        return true;
    }

    qDebug()<< "cannot load json";
    return false;
}

//! Dumps model data into \a jsonObject
bool JsonModel::dumpTo(QJsonObject &jsonObject) const
{
    jsonObject = dump().toObject();
    return true;
}

//! Dumps model data into \a jsonArray
bool JsonModel::dumpTo(QJsonArray &jsonArray) const
{
    jsonArray = dump().toArray();
    return true;
}

//! Dumps model data into \a jsonDocument
bool JsonModel::dumpTo(QJsonDocument &jsonDocument) const
{
    jsonDocument = QJsonDocument(dump().toObject());
    return true;
}

//! Dumps model data into \a byteArray
bool JsonModel::dumpTo(QByteArray &byteArray) const
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

//! Dumps model data into new QJsonValue
QJsonValue JsonModel::dump() const
{
    qDebug() << Q_FUNC_INFO;
    return mRootItem->serialize();
}

/*! \brief Method reimplementation for retrieving single model item data
 *
 * \param index
 * \param role
 * \return data at given \a index as QVariant
 */
QVariant JsonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    JsonObjectBase *item = static_cast<JsonObjectBase*>(index.internalPointer());

    if (role == Qt::DisplayRole)
    {

        if (index.column() == 0)
            return item->name().toVariant();
        if (index.column() == 1)
            return item->value().toVariant();
    }
    return QVariant();
}

//! Reimplemented QAbstractItemModel method to conform this model
QVariant JsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return mHeaders.value(section);
    }
    else
        return QVariant();
}

//! Reimplemented QAbstractItemModel method to conform this model
QModelIndex JsonModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    JsonObjectBase *parentItem;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<JsonObjectBase*>(parent.internalPointer());

    JsonObjectBase *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

//! Reimplemented QAbstractItemModel method to conform this model
QModelIndex JsonModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    JsonObjectBase *childItem = static_cast<JsonObjectBase*>(index.internalPointer());
    JsonObjectBase *parentItem = childItem->parent();

    if (parentItem == mRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

//! Reimplemented QAbstractItemModel method to conform this model
int JsonModel::rowCount(const QModelIndex &parent) const
{
    JsonObjectBase *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<JsonObjectBase*>(parent.internalPointer());

    return parentItem->childCount();
}

//! Reimplemented QAbstractItemModel method to conform this model. Has fixed number of columns = 2
int JsonModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

//! Reimplemented QAbstractItemModel method to conform this model. Adds "Qt::ItemIsEditable" flag to 'leaf' (bottom-level) objects in tree
Qt::ItemFlags JsonModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags itemFlags = QAbstractItemModel::flags(index);
    JsonObjectBase *item = static_cast<JsonObjectBase*>(index.internalPointer());
    if (item->childCount() == 0)
        itemFlags |= Qt::ItemIsEditable;
    return itemFlags;
}

/*! \brief Method reimplementation for updating single model item data
 *
 * \param index
 * \param value
 * \param role
 * \return true on success, false otherwise
 */
bool JsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
    {
        return false;
    }
    JsonObjectBase *item = static_cast<JsonObjectBase*>(index.internalPointer());

    if (item->type() == QJsonValue::Bool)
    {
        item->setValue(QJsonValue(value.toBool()));
    }
    else if (item->type() == QJsonValue::Double)
    {
        item->setValue(QJsonValue(value.toDouble()));
    }
    else if (item->type() == QJsonValue::String)
    {
        item->setValue(QJsonValue(value.toString()));
    }

    dataChanged(index, index, QVector<int>() << role);
    return true;
}

//! Returns pointer to models root item
JsonObjectRoot *JsonModel::rootItem() const
{
    return mRootItem;
}

/*! \brief Informs model about insert row procedure start
 *
 */
void JsonModel::beginAppendRow(const QModelIndex &parent)
{
    beginInsertRows(parent, rowCount(), rowCount() + 1);
}

/*! \brief Informs model about insert row procedure end. As a result, model is refreshed
 *
 */
void JsonModel::endAppendRow()
{
    endInsertRows();
}

