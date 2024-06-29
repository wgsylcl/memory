#ifndef UPLOADPREVIEWIMAGEPROVIDER_H
#define UPLOADPREVIEWIMAGEPROVIDER_H

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

class UploadPreviewImageProvider : public QQuickImageProvider
{
public:
    explicit UploadPreviewImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
signals:

private:
    QImage playimg;
    void loadimage(QUrl,QImage&);
    void loadvideo(QUrl,QImage&);
};

#endif // UPLOADPREVIEWIMAGEPROVIDER_H
