#include "databaseinitializer.h"

DataBaseInitializer::DataBaseInitializer(QObject *parent)
    : QObject{parent}
{
}

void DataBaseInitializer::readremoteinfo()
{
    QFile infofile(runtimedir + "/cache/info.json");
    infofile.open(QIODevice::ReadOnly);
    database->remoteroot = QJsonDocument::fromJson(infofile.readAll()).object();
    infofile.close();
    parseprofile();
    parsepictures();
    parseactivities();
    readlocalversion();
    parsedatabaseinfo();
    infofile.remove();
}

void DataBaseInitializer::downloadremoteinfo()
{
// #define MEMORY_NETWORK_TEST
#ifndef MEMORY_NETWORK_TEST
    QUrl downloadurl(INFOURL);
    Downloader *infodownloader = new Downloader(downloadurl, runtimedir + "/cache/info.json");
    QObject::connect(infodownloader, &Downloader::downloadfinished, this, &DataBaseInitializer::readremoteinfo);
    downloadmanager->adddownloader(infodownloader);
#else
    QUrl downloadurl("https://gitee.com/api/v5/repos/wgsylcl/memorydata007/raw/飞天梦.mp4.data.2?access_token=aad91f98d294fb7ce387995aa7c5b9fb");
    for (int i = 0; i < 100; i++)
    {
        Downloader *infodownloader = new Downloader(downloadurl, runtimedir + QString("/cache/飞天梦%1.mp4.data.2").arg(i));
        downloadmanager->adddownloader(infodownloader);
    }
#endif
}

void DataBaseInitializer::parseprofile()
{
    QJsonObject profiles = database->remoteroot.value("profiles").toObject();
    database->remoteprofileversion = profiles.value("version").toString();
    database->profilereponame = profiles.value("reponame").toString();
}

void DataBaseInitializer::parseactivities()
{
    QJsonArray jarr = database->remoteroot.value("activities").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jactivitie = jarr.at(i).toObject();
        memorybase::Activity activity;
        activity.name = jactivitie.value("name").toString();
        activity.remoteversion = jactivitie.value("version").toString();
        activity.size = jactivitie.value("size").toString();
        QJsonArray reponames = jactivitie.value("reponame").toArray();
        for (int j = 0; j < reponames.count(); j++)
            activity.reponame.push_back(reponames.at(j).toString());
        database->activities.insert(activity.name, activity);
    }
}

void DataBaseInitializer::parsepictures()
{
    QJsonObject pictures = database->remoteroot.value("pictures").toObject();
    database->remotepictureversion = pictures.value("version").toString();
    QJsonArray reponames = pictures.value("reponame").toArray();
    for (int i = 0; i < reponames.count(); i++)
        database->picturereponames.append(reponames.at(i).toString());
}

void DataBaseInitializer::parsedatabaseinfo()
{
    QJsonArray databaseinfos = database->remoteroot.value("users").toArray();
    for(int i=0;i<databaseinfos.count();i++)
    {
        QJsonObject databaseinfo = databaseinfos.at(i).toObject();
        QString username = databaseinfo.value("name").toString();
        QString token = databaseinfo.value("access_token").toString();
        database -> usertokens.insert(username,token);
        QJsonArray reponames = databaseinfo.value("reponames").toArray();
        for(int j=0;j<reponames.count();j++)
        {
            QString reponame = reponames.at(j).toString();
            database -> usernames.insert(reponame,username);
        }
    }
}

void DataBaseInitializer::initialize()
{
    downloadremoteinfo();
}

void DataBaseInitializer::readlocalversion()
{
    QFile localversionfile(runtimedir + "/data/versions.json");
    localversionfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonObject &localjsonroot = database->jsonroot;
    localjsonroot = QJsonDocument::fromJson(localversionfile.readAll()).object();
    localversionfile.close();
    database->localprofileversion = localjsonroot.value("profiles").toString();
    database->localpictureversion = localjsonroot.value("pictures").toString();
    QJsonArray localactivitiesjsonarray = localjsonroot.value("activities").toArray();
    for (int i = 0; i < localactivitiesjsonarray.count(); i++)
    {
        QJsonObject localactivity = localactivitiesjsonarray.at(i).toObject();
        QString name = localactivity.value("name").toString();
        QString version = localactivity.value("version").toString();
        database->localactivityversions.insert(name, version);
        database->activities[name].localversion = version;
    }
}