#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include "base.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libswresample/swresample.h"
#include "libavfilter/avfilter.h"
#include "libswscale/swscale.h"
}

class ImageProvider : public QQuickImageProvider
{
public:
    explicit ImageProvider();
    ~ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
signals:

private:
    QImage playimg;
    void loadimage(QUrl,QImage&);
    void loadvideo(QUrl,QImage&);
};

#endif // IMAGEPROVIDER_H
