#include "downloader.h"

Downloader::Downloader(QUrl downloadurl, QString savepath)
    :downloadurl(downloadurl),savefile(savepath)
{
}

Downloader::~Downloader()
{
    if(reply)
    {
        savefile.close();
        savefile.remove();
        reply -> abort();
        reply -> deleteLater();
        reply = nullptr;
        manager -> deleteLater();
        manager = nullptr;
    }
}

void Downloader::startdownload()
{
    savefile.open(QIODevice::WriteOnly);
    manager = new QNetworkAccessManager();
    reply = manager -> get(QNetworkRequest(downloadurl));
    QObject::connect(reply,&QNetworkReply::readyRead,this,&Downloader::receivedata);
    QObject::connect(reply,&QNetworkReply::finished,this,&Downloader::downloadfinish);
    qDebug() << "start downloading " << downloadurl;
}

void Downloader::receivedata()
{
    savefile.write(reply -> readAll());
}

void Downloader::downloadfinish()
{
    savefile.close();
    reply -> deleteLater();
    reply = nullptr;
    manager -> deleteLater();
    manager = nullptr;
    qDebug() << "download " << downloadurl << " successfully";
    emit downloadfinished(this);
}
