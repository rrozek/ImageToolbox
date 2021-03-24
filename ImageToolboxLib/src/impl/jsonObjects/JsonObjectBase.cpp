#include "JsonObjectBase.h"

//! Constructor
JsonObjectBase::JsonObjectBase(JsonObjectBase *parent)
    : mParent(parent)
    , mKey(QString())
    , mValue(QJsonValue())
    , mType(QJsonValue::Undefined)
{
}

//! Destructor
JsonObjectBase::~JsonObjectBase()
{
    qDeleteAll(mChilds);
}

/*! \brief Retrieves more human-readable name of the object
 *
 * \details Checks for known and defined popular properties from provided dbconf.json
 *          file which can act as a name, retrieves them and returns if possible.
 *          If no such property is present, returns the objects key()
 * \return retrieved object name as QJsonValue
 * \sa key()
 */
QJsonValue JsonObjectBase::name() const
{
//    bool canConvertToInt = false;
//    key().toInt(&canConvertToInt);
//    if (canConvertToInt)
//    {
//        if (childCount() == 1)
//            return child(0)->value();
//        if (childCount() == 0)
//            return value();
//        JsonObjectBase *childWithKey = child(json::DATA_DEF_ID);
//        if (childWithKey != Q_NULLPTR)
//            return childWithKey->value();
//        childWithKey = child(json::TABLE_NAME);
//        if (childWithKey != Q_NULLPTR)
//            return childWithKey->value();
//        childWithKey = child(json::FIELD_NAME);
//        if (childWithKey != Q_NULLPTR)
//            return childWithKey->value();
//    }
    return key();
}

/*! \brief Retrieves JSON path to the object from JSON root item
 *
 * \details Recursively called on treemodel upwards, to the root item.
 * \return retrieved path to tree root item in format like: root.dataObjects[0]fields[1]
 * \sa key(), name(), pathWithNames(), printPaths()
 */
QString JsonObjectBase::path() const
{
    QString retPath = mKey;
    if (mParent != Q_NULLPTR)
    {
        bool isInt = false;
        mKey.toInt(&isInt);
        if (isInt)
            retPath = mParent->path() + "[" + mKey + "]";
        else
            retPath = mParent->path() + "." + mKey;
    }
    return retPath;
}

/*! \brief Retrieves JSON path to the object from JSON root item
 *
 * \details Recursively called on treemodel upwards, to the root item.
 *          Returns more appealing and readable form of path
 * \return retrieved path to tree root item in format like: root.dataObjects.SystemGroup.fields.Id
 * \sa name(), key(), path(), printPaths()
 */
QString JsonObjectBase::pathWithNames() const
{
    QString retPath = name().toString();
    if (mParent != Q_NULLPTR)
    {
        retPath = mParent->pathWithNames() + "." + name().toString();
    }
    return retPath;
}

//! Helper method for printing all child paths
void JsonObjectBase::printPaths() const
{
    for (JsonObjectBase* child : mChilds)
    {
        child->printPaths();
    }
    qDebug() << path();
}

//! Helper method for printing all child paths with names
void JsonObjectBase::printPathWithNames() const
{
    for (JsonObjectBase* child : mChilds)
    {
        child->printPathWithNames();
    }
    qDebug() << pathWithNames();
}

/*! \brief Retrieves object pointer in the tree by crawling from root item downwards
 *
 * \param path - lookup path of item
 * \note \a path must be relative to root item.
 * \return retrieved pointer to requested object
 */
JsonObjectBase *JsonObjectBase::getObjectByPath(const QString &path) const
{
    JsonObjectBase* obj = Q_NULLPTR;
    for (JsonObjectBase* child : mChilds)
    {
        if (child->path() == path)
            obj = child;
        if (obj != Q_NULLPTR)
            return obj;
        obj = child->getObjectByPath(path);
    }
    return obj;
}

//! Equal operator
bool JsonObjectBase::operator==(const JsonObjectBase &other) const
{
    return isEqual(other);
}

/*! \brief Checks if object is equal with another object
 *
 * \param other - object, to which this object is compared
 * \note supports error reporting
 * \return true, if objects are equal
 */
bool JsonObjectBase::isEqual(const JsonObjectBase &other) const
{
    bool ret = true;
    ret = mValue == other.mValue;
    if (!ret)
        return false;
    for (QString key : childNames())
    {
        ret = child(key)->isEqual(*other.child(key));
        if (!ret)
            return false;
    }
    return ret;
}

/*! \brief Checks if object contains another object
 *
 * \param obj - object which is expected to be enclosed in this object
 * \note supports error reporting
 * \return true, if this object contains \a obj
 */
bool JsonObjectBase::contains(const JsonObjectBase &obj) const
{
    bool ret = true;
    ret = mType == obj.mType;
    if (!ret)
        return false;
    if (mType != QJsonValue::Object && mType != QJsonValue::Array && obj.mType != QJsonValue::Object && obj.mType != QJsonValue::Array)
        return mValue == obj.mValue;
    for (QString key : obj.childNames())
    {
        if (child(key) == Q_NULLPTR)
            continue;
        ret = *child(key) == *obj.child(key);
        if (!ret)
            false;
    }
    return ret;
}

//! Appends child to item
void JsonObjectBase::appendChild(JsonObjectBase *item)
{
    mChilds.append(item);
}

void JsonObjectBase::removeChild(int row)
{
    if (row >= mChilds.size() || row < 0)
        return;
    JsonObjectBase *childToRemove = mChilds.takeAt(row);
    delete childToRemove;
    childToRemove = Q_NULLPTR;
}

void JsonObjectBase::removeChild(const QString &key)
{
    for (int i = 0; i < mChilds.size(); i++)
    {
        if (mChilds[i]->name() == key)
        {
            removeChild(i);
            return;
        }
    }
}

//! Retrieves child at given \a row
JsonObjectBase *JsonObjectBase::child(int row) const
{
    if (mChilds.size() > row)
        return mChilds.value(row);
    return Q_NULLPTR;
}

//! Retrieves child with given \a key
JsonObjectBase *JsonObjectBase::child(const QString& key) const
{
    for (JsonObjectBase* child : mChilds)
    {
        if (child->name() == key)
            return child;
    }
    return Q_NULLPTR;
}

void JsonObjectBase::setParent(JsonObjectBase *parent)
{
    mParent = parent;
}

//! Returns pointer to parent item of this item
JsonObjectBase *JsonObjectBase::parent()
{
    return mParent;
}

//! Returns number of this items children
int JsonObjectBase::childCount() const
{
    return mChilds.size();
}

//! Returns list of this items children names
QStringList JsonObjectBase::childNames() const
{
    QStringList list;
    for (JsonObjectBase* child : mChilds)
    {
        list.append(child->name().toString());
    }
    return list;
}

//! Returns row of this item
int JsonObjectBase::row() const
{
    if (mParent)
        return mParent->mChilds.indexOf(const_cast<JsonObjectBase*>(this));

    return 0;
}

//! Sets item key
void JsonObjectBase::setKey(const QString &key)
{
    mKey = key;
}

//! Sets item value
void JsonObjectBase::setValue(const QJsonValue &value)
{
    mValue = value;
}

//! Sets item type
void JsonObjectBase::setType(const QJsonValue::Type &type)
{
    mType = type;
}

//! Retrieves item key
QString JsonObjectBase::key() const
{
    return mKey;
}

//! Retrieves item value
QJsonValue JsonObjectBase::value() const
{
    return mValue;
}

//! Retrieves item type
QJsonValue::Type JsonObjectBase::type() const
{
    return mType;
}

/*! \brief Loads \a value to this object, including all childs
 *
 * \param value
 * \param parent
 * \return pointer to created JsonObjectBase object
 */
JsonObjectBase* JsonObjectBase::load(const QJsonValue& value, JsonObjectBase* parent)
{
    JsonObjectBase * rootItem = Q_NULLPTR;
    if (parent == Q_NULLPTR)
        rootItem = this;
    else
        rootItem = new JsonObjectBase(parent);
    rootItem->setKey("root");

    if ( value.isObject())
    {

        //Get all QJsonValue childs
        for (QString key : value.toObject().keys()){
            QJsonValue v = value.toObject().value(key);
            JsonObjectBase * child = load(v,rootItem);
            child->setKey(key);
            child->setType(v.type());
            rootItem->appendChild(child);
        }

    }
    else if ( value.isArray())
    {
        //Get all QJsonValue childs
        int index = 0;
        for (QJsonValue v : value.toArray()){

            JsonObjectBase * child = load(v,rootItem);
            child->setKey(QString::number(index));
            child->setType(v.type());
            rootItem->appendChild(child);
            ++index;
        }
    }
    else
    {
        rootItem->setValue(value);
        rootItem->setType(value.type());
    }
    return rootItem;
}

/*! \brief Dumps this object to \a jsonValue as its child (property/item)
 *
 * \param jsonValue - value where this object is written into
 * \return true on success, false otherwise
 */
bool JsonObjectBase::dumpTo(QJsonValue &jsonValue) const
{
    for (JsonObjectBase* child : mChilds)
    {
        QJsonValue childValue;
        if (child->type() == QJsonValue::Object)
        {
            childValue = QJsonObject();
            child->dumpTo(childValue);
        }
        else if (child->type() == QJsonValue::Array)
        {
            childValue = QJsonArray();
            child->dumpTo(childValue);
        }
        else
        {
            childValue = child->value();
        }

        if (jsonValue.isArray())
        {
            QJsonArray arr = jsonValue.toArray();
            arr.append(childValue);
            jsonValue = arr;
        }
        else if (jsonValue.isObject())
        {
            QJsonObject obj = jsonValue.toObject();
            obj.insert(child->key(), childValue);
            jsonValue = obj;
        }
        else
        {
            qWarning() << __LINE__ << "AFAIK this should not happen. jsonValue must be either Obj or Array";
        }
    }

    if (!value().isNull() && !value().isUndefined())
    {
        if (jsonValue.isArray())
        {
            QJsonArray arr = jsonValue.toArray();
            arr.append(value());
            jsonValue = arr;
        }
        else if (jsonValue.isObject())
        {
            QJsonObject obj = jsonValue.toObject();
            obj.insert(key(), value());
            jsonValue = obj;
        }
        else
        {
            qDebug() << jsonValue;
            qWarning() << __LINE__ << "AFAIK this should not happen. jsonValue must be either Obj or Array";
        }
    }
    return true;
}

/*! \brief Helper method extending QtJson framework with possibility to modify QJsonValue of an object
 *
 * \details By default, QtJson module supports read-only access to object. this method adds possibility to modify QtJson object
 * \param destValue - value to be modified
 * \param path - path to modified property (example: fields[0].Id.name)
 * \param newValue - value to be written to the \a destValue at \a path
 */
void JsonObjectBase::modifyJsonValue(QJsonValue& destValue, const QString& path, const QJsonValue& newValue)
{
    const int indexOfDot = path.indexOf('.');
    const QString dotPropertyName = path.left(indexOfDot);
    const QString dotSubPath = indexOfDot > 0 ? path.mid(indexOfDot + 1) : QString();

    const int indexOfSquareBracketOpen = path.indexOf('[');
    const int indexOfSquareBracketClose = path.indexOf(']');

    const int arrayIndex = path.mid(indexOfSquareBracketOpen + 1, indexOfSquareBracketClose - indexOfSquareBracketOpen - 1).toInt();

    const QString squareBracketPropertyName = path.left(indexOfSquareBracketOpen);
    const QString squareBracketSubPath = indexOfSquareBracketClose > 0 ? (path.mid(indexOfSquareBracketClose + 1)[0] == '.' ? path.mid(indexOfSquareBracketClose + 2) : path.mid(indexOfSquareBracketClose + 1)) : QString();

    // determine what is first in path. dot or bracket
    bool useDot = true;
    if (indexOfDot >= 0) // there is a dot in path
    {
        if (indexOfSquareBracketOpen >= 0) // there is squarebracket in path
        {
            if (indexOfDot > indexOfSquareBracketOpen)
                useDot = false;
            else
                useDot = true;
        }
        else
            useDot = true;
    }
    else
    {
        if (indexOfSquareBracketOpen >= 0)
            useDot = false;
        else
            useDot = true; // acutally, id doesn't matter, both dot and square bracket don't exist
    }

    QString usedPropertyName = useDot ? dotPropertyName : squareBracketPropertyName;
    QString usedSubPath = useDot ? dotSubPath : squareBracketSubPath;

    QJsonValue subValue;
    if (destValue.isArray())
        subValue = destValue.toArray()[usedPropertyName.toInt()];
    else if (destValue.isObject())
        subValue = destValue.toObject()[usedPropertyName];
    else
        qDebug() << "oh, what should i do now with the following value?! " << destValue;

    if(usedSubPath.isEmpty())
    {
        subValue = newValue;
    }
    else
    {
        if (subValue.isArray())
        {
            QJsonArray arr = subValue.toArray();
            QJsonValue arrEntry = arr[arrayIndex];
            modifyJsonValue(arrEntry,usedSubPath,newValue);
            arr[arrayIndex] = arrEntry;
            subValue = arr;
        }
        else if (subValue.isObject())
            modifyJsonValue(subValue,usedSubPath,newValue);
        else
            subValue = newValue;
    }

    if (destValue.isArray())
    {
        QJsonArray arr = destValue.toArray();
        arr[arrayIndex] = subValue;
        destValue = arr;
    }
    else if (destValue.isObject())
    {
        QJsonObject obj = destValue.toObject();
        obj[usedPropertyName] = subValue;
        destValue = obj;
    }
    else
        destValue = newValue;
}

/*! \brief Helper method extending QtJson framework with possibility to modify QJsonValue of an object
 *
 * \details By default, QtJson module supports read-only access to object. this method adds possibility to modify QtJson object
 * \param doc - json document to be modified
 * \param path - path to modified property (example: fields[0].Id.name)
 * \param newValue - value to be written to the \a doc at \a path
 */
void JsonObjectBase::modifyJsonValue(QJsonDocument& doc, const QString& path, const QJsonValue& newValue)
{
    QJsonValue val;
    if (doc.isArray())
        val = doc.array();
    else
        val = doc.object();

    modifyJsonValue(val,path,newValue);

    if (val.isArray())
        doc = QJsonDocument(val.toArray());
    else
        doc = QJsonDocument(val.toObject());
}

