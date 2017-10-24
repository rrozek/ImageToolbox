#include "Utils.h"

namespace GSNImageToolBox
{
namespace common
{

QString Utils::filterImagesRegex = "/\\.(tif?f|png|eps)$/i";
QStringList Utils::filterImagesList = Utils::initList();

Utils::Utils()
{

}

QStringList Utils::initList()
{
    QStringList fileFilterList {
        { "*.tif" }
        , { "*.tiff" }
        , { "*.eps" }
        , { "*.png" }
        , { "*.TIF" }
        , { "*.TIFF" }
        , { "*.EPS" }
        , { "*.PNG" }
    };
    return fileFilterList;
}


} // namespace common
} // namespace GSNImageToolBox

