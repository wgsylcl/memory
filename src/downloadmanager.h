#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QQueue>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include "downloader.h"
#include "database.h"

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);
    ~DownloadManager();
    void adddownloader(Downloader* newdownloader);
    static DownloadManager* instance();
signals:

public slots:
    void downloadfinish(Downloader* from);

private:
    QQueue<Downloader*> waitingdownloaders;
    QList<Downloader*> runningdownloaders;
    const int maxrunningdownloads;
    QMutex lock;
    static DownloadManager* _instance;
};

#endif // DOWNLOADMANAGER_H
