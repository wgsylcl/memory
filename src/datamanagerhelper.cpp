#include "datamanagerhelper.h"

DataManagerHelper::DataManagerHelper(QObject *parent)
    : QObject{parent}
{}

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
    ProfilePictureUpdater* updater = ProfilePictureUpdater::instance();
    updater->run();
}
