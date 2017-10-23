#pragma once

#include <QDebug>
#include <QString>

namespace common
{


class Utils
{
public:
    Utils();

    static QString filterImagesRegex;
    static QStringList filterImagesList;

private:
    static QStringList initList();
};

} // namespace common
