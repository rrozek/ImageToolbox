#ifndef JSONOBJECTBASE_H
#define JSONOBJECTBASE_H

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QDebug>

/*! \class JsonObjectBase
 *  \brief Basic treeview object class
 *  representing single JSON object/value
 *
 * \details Each object has three basic properties \ref mKey and \ref mValue and a list of \ref mChilds.
 * If object has children, then its \ref mValue is null and an appropriate \ref mType is set either to Object or Array
 */

class JsonObjectBase
{
public:
    explicit JsonObjectBase(JsonObjectBase * parent = 0);
    virtual ~JsonObjectBase();

    QJsonValue name() const;
    QString path() const;
    QString pathWithNames() const;
    void printPaths() const;
    void printPathWithNames() const;
    JsonObjectBase* getObjectByPath(const QString& path) const;

    bool operator==(const JsonObjectBase& other) const;
    bool isEqual(const JsonObjectBase& other) const;
    bool contains(const JsonObjectBase& obj) const;

    void appendChild(JsonObjectBase * item);
    void removeChild(int row);
    void removeChild(const QString &key);
    JsonObjectBase *child(int row) const;
    JsonObjectBase *child(const QString &key) const;
    void setParent(JsonObjectBase *parent);
    JsonObjectBase *parent();
    int childCount() const;
    QStringList childNames() const;

    int row() const;
    virtual void setKey(const QString& key);
    virtual void setValue(const QJsonValue& value);
    void setType(const QJsonValue::Type &type);
    virtual QString key() const;
    virtual QJsonValue value() const;
    QJsonValue::Type type() const;

    JsonObjectBase* load(const QJsonValue& value, JsonObjectBase * parent = 0);

    // editable model additions
    bool dumpTo(QJsonValue &jsonValue) const;

    static void modifyJsonValue(QJsonValue &destValue, const QString& path, const QJsonValue& newValue);
    static void modifyJsonValue(QJsonDocument& doc, const QString& path, const QJsonValue& newValue);
protected:
    JsonObjectBase* mParent;

private:
    QList<JsonObjectBase*> mChilds;
    QString mKey;
    QJsonValue mValue;
    QJsonValue::Type mType;
};
#endif // JSONOBJECTBASE_H
