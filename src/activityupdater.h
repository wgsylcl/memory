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
    void updaterepo(QString activityname,QString reponame,QStringList filelist);
    void dealdownloadfinished(QString activityname);
    static ActivityUpdater* instance();
    Q_INVOKABLE bool is_updating(QString activityname);
signals:
    void updatefinished(QString activityname);
private:
    static ActivityUpdater* _instacnce;
    QMap<QString,int> taskcount,repocount;
    QMutex lock;
    QSet<QString> updatinglist;
};

#endif // ACTIVITYUPDATER_H
