#include "database.h"

DataBase *DataBase::_instance = nullptr;

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    _instance = this;
}

DataBase::~DataBase()
{

}

DataBase *DataBase::instance()
{
    return _instance;
}

QStringList DataBase::getAllLocalActivitieNames()
{
    QStringList ret;
    for(auto it = localactivityversions.begin();it != localactivityversions.end();it++)
        ret.append(it.key());
    return ret;
}

QStringList DataBase::getAllRemoteActivitieNames()
{
    QStringList ret;
    for(auto it = activities.begin();it != activities.end();it++)
        ret.append(it.key());
    return ret;
}

memorybase::Activity &DataBase::getactivity(QString activityname)
{
    return activities[activityname];
}

QString DataBase::getlocalprofilepictureversion(void)
{
    return localprofileversion;
}

QString DataBase::getremoteprofilepictureversion(void)
{
    return remotepictureversion;
}

QString DataBase::getlocalactivityversionbyname(QString name)
{
    return activities[name].loaclversion;
}

QString DataBase::getremoteactivityversionbyname(QString name)
{
    return activities[name].remoteversion;
}

QString DataBase::getprofilereponame(void)
{
    return profilereponame;
}

QStringList DataBase::getpicturereponames(void)
{
    return picturereponames;
}

void DataBase::synclocalprofilepictureversion(void)
{
    localprofileversion = localpictureversion = remoteprofileversion;
    jsonroot["profiles"] = localprofileversion;
    jsonroot["pictures"] = localpictureversion;
    saveinfo();
}

void DataBase::removelocalactivityversionbyname(QString name)
{
    activities[name].loaclversion = "0.0.0";
    localactivityversions.remove(name);
    QJsonArray jactivities;
    for(auto it = localactivityversions.begin();it != localactivityversions.end();it++)
    {
        QJsonObject localactivity;
        localactivity["name"] = it.key();
        localactivity["version"] = it.value();
        jactivities.append(localactivity);
    }
    jsonroot["activities"] = jactivities;
    saveinfo();
}

void DataBase::saveinfo()
{
    QJsonDocument jdoc(jsonroot);
    QFile file(runtimedir + "/data/versions.json");
    file.open(QIODevice::WriteOnly);
    file.write(jdoc.toJson());
    file.close();
}
