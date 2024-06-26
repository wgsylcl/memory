#ifndef DATAMANAGERHELPER_H
#define DATAMANAGERHELPER_H

#include "base.h"
#include "database.h"
#include "profilepictureupdater.h"
#include "activityupdater.h"

class DataManagerHelper : public QObject
{
    Q_OBJECT
public:
    explicit DataManagerHelper(QObject *parent = nullptr);
    Q_INVOKABLE QString getlocalprofilepictureversion(void);
    Q_INVOKABLE QString getremoteprofilepictureversion(void);
    Q_INVOKABLE QString getlocalactivityversionbyname(QString name);
    Q_INVOKABLE QString getremoteactivityversionbyname(QString name);
    Q_INVOKABLE void removelocalactivitybyname(QString name);
    Q_INVOKABLE QStringList getallactivitiynames(void);
    Q_INVOKABLE void startsyncprofilepicture(void);
    Q_INVOKABLE void startsyncactivity(QString activityname);
    Q_INVOKABLE QString getactivitysize(QString activityname);
};

#endif // DATAMANAGERHELPER_H
