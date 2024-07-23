#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include "base.h"
#include "firstframepicker.h"

class ImageProvider : public QQuickImageProvider
{
public:
    explicit ImageProvider();
    ~ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
signals:

private:
    QImage playimg;
    void loadimage(QUrl, QImage &);
    void loadvideo(QUrl, QImage &);
    void addwatermark(QString, QImage &);
};

#endif // IMAGEPROVIDER_H
