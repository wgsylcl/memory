#include "bigfiledownloader.h"

BigFileDownloader::BigFileDownloader(QString reponame, QString filename, QString savepath, QObject *parent)
    : QObject{parent}, reponame(reponame), filename(filename), savepath(savepath), filecount(0), finishedcount(0)
{
}

void BigFileDownloader::startdownload()
{
    Downloader *headfiledownloader = new Downloader(database->generaterequesturl(reponame, filename + ".0"), runtimedir + "/cache/" + filename + ".0");
    QObject::connect(headfiledownloader, &Downloader::downloadfinished, this, &BigFileDownloader::receiveheadfile);
    downloadmanager->adddownloader(headfiledownloader);
}

void BigFileDownloader::mergefile()
{
    QFile bigfile(savepath);
    bigfile.open(QIODevice::WriteOnly);
    for (int i = 1; i <= filecount; i++)
    {
        QFile partfile(runtimedir + "/cache/" + filename + "." + memorybase::to_qstring(i));
        partfile.open(QIODevice::ReadOnly);
        bigfile.write(partfile.readAll());
        partfile.close();
        partfile.remove();
    }
    bigfile.close();
    emit downloadfinished();
}

void BigFileDownloader::receivepartfile()
{
    QMutexLocker locker(&lock);
    if ((++finishedcount) == filecount)
        mergefile();
}

void BigFileDownloader::receiveheadfile()
{
    QFile headfile(runtimedir + "/cache/" + filename + ".0");
    headfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream qin(&headfile);
    qin >> filecount;
    headfile.close();
    headfile.remove();
    for (int i = 1; i <= filecount; i++)
    {
        Downloader *headfiledownloader = new Downloader(database->generaterequesturl(reponame, filename + "." + memorybase::to_qstring(i)), runtimedir + "/cache/" + filename + "." + memorybase::to_qstring(i));
        QObject::connect(headfiledownloader, &Downloader::downloadfinished, this, &BigFileDownloader::receivepartfile);
        downloadmanager->adddownloader(headfiledownloader);
    }
}
