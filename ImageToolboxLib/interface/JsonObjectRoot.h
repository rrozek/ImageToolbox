#pragma once
#include "ImageToolboxLib_global.h"

#include "JsonObjectBase.h"

/*! \class JsonObjectRoot
 *  \brief Root object of a JsonModel
 *
 * \details This class is instantiated only once per model.
 *          It extends the standard JsonObjectBase functionality with
 *          Json integrity rules which need to be checked upon model change.
 */
class IMAGETOOLBOXLIB_SHARED_EXPORT JsonObjectRoot : public JsonObjectBase
{
public:
    explicit JsonObjectRoot(JsonObjectBase * parent = 0);
    virtual ~JsonObjectRoot();

    virtual QJsonValue serialize() const;
    virtual void deserialize(const QJsonValue &srcValue);

private:
};

