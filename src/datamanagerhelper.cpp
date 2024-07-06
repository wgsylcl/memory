#include "datamanagerhelper.h"

DataManagerHelper::DataManagerHelper(QObject *parent)
    : QObject{parent}
{
}

Q_INVOKABLE QString DataManagerHelper::getlocalprofilepictureversion(void)
{
    return database->getlocalprofilepictureversion();
}

Q_INVOKABLE QString DataManagerHelper::getremoteprofilepictureversion(void)
{
    return database->getremoteprofilepictureversion();
}

Q_INVOKABLE QString DataManagerHelper::getlocalactivityversionbyname(QString name)
{
    return database->getlocalactivityversionbyname(name);
}

Q_INVOKABLE QString DataManagerHelper::getremoteactivityversionbyname(QString name)
{
    return database->getremoteactivityversionbyname(name);
}

Q_INVOKABLE void DataManagerHelper::removelocalactivitybyname(QString name)
{
    QDir activitydir(runtimedir + "/data/activities/" + name);
    activitydir.removeRecursively();
    database->removelocalactivityversionbyname(name);
}

Q_INVOKABLE QStringList DataManagerHelper::getallactivitiynames(void)
{
    return database->getAllRemoteActivitieNames();
}

Q_INVOKABLE void DataManagerHelper::startsyncprofilepicture(void)
{
    ProfilePictureUpdater::instance()->run();
}

Q_INVOKABLE void DataManagerHelper::startsyncactivity(QString activityname)
{
    ActivityUpdater::instance()->startupdate(activityname);
}

Q_INVOKABLE QString DataManagerHelper::getactivitysize(QString activityname)
{
    return memorybase::getdirsize(runtimedir + "/datas/activities/" + activityname);
}
