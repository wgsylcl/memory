#include "downloader.h"

Downloader::Downloader(QUrl downloadurl, QString savepath)
    :downloadurl(downloadurl),savefile(savepath)
{
}

Downloader::~Downloader()
{
    if(reply)
    {
        savefile.cancelWriting();
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
}

void Downloader::receivedata()
{
    savefile.write(reply -> readAll());
}

void Downloader::downloadfinish()
{
    if(reply->error() == QNetworkReply::NoError)
    {
        savefile.commit();
        emit downloadfinished(this);
    }
    else
    {
        qDebug() << "Network error occurred:" << reply->errorString();
        emit downloadfailed(this);
    }
    reply -> deleteLater();
    reply = nullptr;
    manager -> deleteLater();
    manager = nullptr;
}
