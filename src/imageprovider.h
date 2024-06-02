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
    Q_OBJECT
public:
    explicit ImageProvider(QObject *parent = nullptr);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
signals:

private:
    QImage image,playimg;
    void loadimage(QUrl);
    void loadvideo(QUrl);
};

#endif // IMAGEPROVIDER_H
