#ifndef ACTIVITYUPDATER_H
#define ACTIVITYUPDATER_H

#include "bigfiledownloader.h"
#include "filelistreader.h"
#include "database.h"

class ActivityUpdater : public QObject
{
    Q_OBJECT
public:
    explicit ActivityUpdater(QObject *parent = nullptr);
    void startupdate(QString activityname);
    void updaterepo(QString activityname,QString reponame,QStringList filelist,int idx);
    void dealdownloadfinished(QString activityname,int idx);
    void dealdownloadfailed(QString activityname,int idx);
    static ActivityUpdater* instance();
    Q_INVOKABLE bool is_updating(QString activityname);
signals:
    void updatefinished(QString activityname);
    void updatefailed(QString activityname);
private:
    static ActivityUpdater* _instacnce;
    QMap<QString,QMap<int,int>> taskcount,repocount,updatefail;
    QMap<QString,int> nidx;
    QMutex lock;
    QSet<QString> updatinglist;
};

#endif // ACTIVITYUPDATER_H
