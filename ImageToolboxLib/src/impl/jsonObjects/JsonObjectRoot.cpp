#include "JsonObjectRoot.h"

//! Constructor
JsonObjectRoot::JsonObjectRoot(JsonObjectBase *parent)
    : JsonObjectBase(parent)
{
}

//! Destructor
JsonObjectRoot::~JsonObjectRoot()
{

}

/*! \brief Writes object content into single QJsonValue
 *
 * \return This item dumped to QJsonValue
 */
QJsonValue JsonObjectRoot::serialize() const
{
    QJsonValue val;
    if (type() == QJsonValue::Array)
        val = QJsonArray();
    else
        val = QJsonObject();
    dumpTo(val);
    return val;
}

/*! \brief Reads \a srcValue and creates tree model of it
 *
 * \param srcValue - value to be read
 */
void JsonObjectRoot::deserialize(const QJsonValue &srcValue)
{
    load(srcValue, Q_NULLPTR);
//    printPaths();
//    printPathWithNames();
}

