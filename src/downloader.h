#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "base.h"
#include "qaesencryption.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QUrl downloadurl,QString savepath);
    ~Downloader();
    void startdownload();

signals:
    void downloadfinished(Downloader* from);

public slots:
    void receivedata();
    void downloadfinish();

private:
    QUrl downloadurl;
    QFile savefile;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};

#endif // DOWNLOADER_H
