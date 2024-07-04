#include "downloadmanager.h"

DownloadManager *DownloadManager::_instance = nullptr;

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}, maxrunningdownloads(8)
{
    _instance = this;
}

DownloadManager::~DownloadManager()
{
    QMutexLocker locker(&lock);
    for (auto i : runningdownloaders)
        i->deleteLater();
    runningdownloaders.clear();
    for (auto i : waitingdownloaders)
        i->deleteLater();
    waitingdownloaders.clear();
}

void DownloadManager::adddownloader(Downloader *newdownloader)
{
    QMutexLocker locker(&lock);
    QObject::connect(newdownloader, &Downloader::downloadfinished, this, &DownloadManager::downloadfinish);
    QObject::connect(newdownloader, &Downloader::downloadfailed, this, &DownloadManager::downloadfinish);
    if (runningdownloaders.size() < maxrunningdownloads)
    {
        runningdownloaders.append(newdownloader);
        newdownloader->startdownload();
    }
    else
    {
        waitingdownloaders.enqueue(newdownloader);
    }
}

DownloadManager *DownloadManager::instance()
{
    return _instance;
}

void DownloadManager::downloadfinish(Downloader *from)
{
    QMutexLocker locker(&lock);
    runningdownloaders.removeOne(from);
    from->deleteLater();
    from = nullptr;
    if (waitingdownloaders.size())
    {
        Downloader *downloader = waitingdownloaders.head();
        waitingdownloaders.dequeue();
        runningdownloaders.append(downloader);
        downloader->startdownload();
    }
}
