#ifndef BIGFILEDOWNLOADER_H
#define BIGFILEDOWNLOADER_H

#include "downloadmanager.h"

class BigFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit BigFileDownloader(QString reponame,QString filename,QString savepath,QObject *parent = nullptr);
    void startdownload();
    void mergefile();
public slots:
    void receiveheadfile();
    void receivepartfile();
    void receivefailed();
signals:
    void downloadfinished();
    void downloadfailed();
private:
    QString reponame,filename,savepath;
    QMutex lock;
    int filecount,finishedcount;
    bool downloadfail;
};

#endif // BIGFILEDOWNLOADER_H
