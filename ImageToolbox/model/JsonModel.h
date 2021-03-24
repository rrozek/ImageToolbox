#pragma once

#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QIODevice>

class JsonObjectRoot;
/*! \class JsonModel
 *  \brief Subclass of QAbstractItemModel translating JSON object into QTreeview model
 *
 */
class JsonModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit JsonModel(QObject *parent = 0);
    virtual ~JsonModel();

    bool load(const QString& fileName);
    bool load(QIODevice* device);
    bool loadJson(const QByteArray& json);
    bool loadJson(const QJsonDocument& jsonDoc);

    void clear();

    bool dumpTo(QJsonObject &jsonObject) const;
    bool dumpTo(QJsonArray &jsonArray) const;
    bool dumpTo(QJsonDocument &jsonDocument) const;
//    bool dumpTo(QByteArray &byteArray) const;

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    // editable model additions
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    JsonObjectRoot* rootItem() const;

    void beginAppendRow(const QModelIndex &parent);
    void endAppendRow();

private:
    QJsonValue dump() const;

    JsonObjectRoot* mRootItem;
    QJsonDocument mDocument;
    QStringList mHeaders;

};

