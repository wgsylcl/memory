#ifndef PROFILEPICTUREUPDATER_H
#define PROFILEPICTUREUPDATER_H

#include "filelistreader.h"
#include "database.h"
#include "bigfiledownloader.h"

class ProfilePictureUpdater : public QObject
{
    Q_OBJECT
public:
    explicit ProfilePictureUpdater(QObject *parent = nullptr);
    Q_INVOKABLE void run();
    static ProfilePictureUpdater* instance(void);
public slots:
    void updateprofile(QStringList filelist,int idx);
    void updatepicture(QString reponame,QStringList filelist,int idx);
    void dealdownloadfinished(int idx);
    void dealdownloadfailed(int idx);
    Q_INVOKABLE bool is_updating();
signals:
    void updatefinished();
    void updatefailed();
private:
    QMutex lock;
    QMap<int,int> taskcount;
    int nidx;
    bool updating;
    QMap<int,bool> updatefail;
    static ProfilePictureUpdater *_instance;
};

#endif // PROFILEPICTUREUPDATER_H
