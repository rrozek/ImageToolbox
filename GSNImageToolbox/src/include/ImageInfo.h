#pragma once

#include <QObject>
#include <QSize>
#include "Common.h"


namespace GSNImageToolBox
{

class ImageInfo : public QObject
{
    Q_OBJECT
public:
    explicit ImageInfo(QObject *parent = nullptr);

    bool isContainer() const;
    quint8 getImagesCount() const;
    const ImageInfo& getImageInfo(quint8 imageNumber = 0) const;
    bool hasThumbnail() const;

signals:

public slots:

private:
    QList<ImageInfo> m_images;
//    Exiv2::ExifData m_exif;
    QSize m_thumbSize;
    QSize m_imageSize;
    quint8 m_bitsPerPixel;
    Common::EColorSpace m_colorSpace;
    Common::EImageFormat m_format;
};

} // namespace GSNImageToolBox

