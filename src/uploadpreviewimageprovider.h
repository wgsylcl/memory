#ifndef UPLOADPREVIEWIMAGEPROVIDER_H
#define UPLOADPREVIEWIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include "base.h"
#include "firstframepicker.h"

class UploadPreviewImageProvider : public QQuickImageProvider
{
public:
    explicit UploadPreviewImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
signals:

private:
    QImage playimg;
    void loadimage(QUrl, QImage &);
    void loadvideo(QUrl, QImage &);
    void addwatermark(QString, QImage &);
};

#endif // UPLOADPREVIEWIMAGEPROVIDER_H
