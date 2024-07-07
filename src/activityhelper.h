#ifndef ACTIVITYHELPER_H
#define ACTIVITYHELPER_H

#include "database.h"
#include <algorithm>
#include <random>

class ActivityHelper : public QObject
{
    Q_OBJECT
public:
    explicit ActivityHelper(QObject *parent = nullptr);
    ~ActivityHelper();
    Q_INVOKABLE QStringList getAllActivities(void);
    Q_INVOKABLE QStringList getAllRemoteActivities(void);
    Q_INVOKABLE int readAllMedia(QString);
    Q_INVOKABLE QUrl getMediaPath(int index);
signals:

private:
    QStringList mediapaths;
    QString getfullpath(memorybase::Activity activity);
};

#endif // ACTIVITYHELPER_H
