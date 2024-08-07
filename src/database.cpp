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
    for (auto it = localactivityversions.begin(); it != localactivityversions.end(); it++)
        ret.append(it.key());
    return ret;
}

QStringList DataBase::getAllRemoteActivitieNames()
{
    QStringList ret;
    for (auto it = activities.begin(); it != activities.end(); it++)
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
    return activities[name].localversion;
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

QStringList DataBase::getactivityreponames(QString activityname)
{
    return activities[activityname].reponame;
}

void DataBase::synclocalprofilepictureversion(void)
{
    localprofileversion = (localpictureversion = remoteprofileversion);
    jsonroot["profiles"] = localprofileversion;
    jsonroot["pictures"] = localpictureversion;
    saveinfo();
}

void DataBase::syncactivityversion(QString activityname)
{
    localactivityversions[activityname] = (activities[activityname].localversion = activities[activityname].remoteversion);
    QJsonArray jactivities;
    for (auto it = localactivityversions.begin(); it != localactivityversions.end(); it++)
    {
        QJsonObject localactivity;
        localactivity["name"] = it.key();
        localactivity["version"] = it.value();
        jactivities.append(localactivity);
    }
    jsonroot["activities"] = jactivities;
    saveinfo();
}

void DataBase::removelocalactivityversionbyname(QString name)
{
    activities[name].localversion = "0.0.0";
    localactivityversions.remove(name);
    QJsonArray jactivities;
    for (auto it = localactivityversions.begin(); it != localactivityversions.end(); it++)
    {
        QJsonObject localactivity;
        localactivity["name"] = it.key();
        localactivity["version"] = it.value();
        jactivities.append(localactivity);
    }
    jsonroot["activities"] = jactivities;
    saveinfo();
}

QUrl DataBase::generaterequesturl(QString reponame, QString filepath)
{
    return QUrl(QString("https://gitee.com/api/v5/repos/%1/%2/raw/%3?access_token=%4").arg(usernames[reponame], reponame, filepath, usertokens[usernames[reponame]]));
}

QUrl DataBase::generatefilelistrequesturl(QString reponame)
{
    return QUrl(QString("https://gitee.com/api/v5/repos/%1/%2/contents?access_token=%4").arg(usernames[reponame], reponame, usertokens[usernames[reponame]]));
}

QString DataBase::getlatestapplicationversion()
{
    return latestapplicationversion;
}

QStringList DataBase::gettips()
{
    if(tips.empty()) return QStringList();
    for (int t = 0; t < 18380; t++)
    {
        static QRandomGenerator *generator = QRandomGenerator::global();
        int i = generator->bounded(0, tips.size() - 1);
        int j = generator->bounded(0, tips.size() - 1);
        memorybase::swap(tips[i], tips[j]);
    }
    return tips;
}

bool DataBase::is_maintaining()
{
    return maintaining;
}

QString DataBase::getdownloadurlpath(QString platform)
{
    return downloadurlpath[platform];
}

void DataBase::saveinfo()
{
    QJsonDocument jdoc(jsonroot);
    QFile file(runtimedir + "/data/versions.json");
    file.open(QIODevice::WriteOnly);
    file.write(jdoc.toJson());
    file.close();
}
