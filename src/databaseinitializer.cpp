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
    database->latestapplicationversion = database->remoteroot.value("appversion").toString();
    parseprofile();
    parsepictures();
    parseactivities();
    readlocalversion();
    parsedatabaseinfo();
    parsetips();
    infofile.remove();
    emit initializefinished();
}

void DataBaseInitializer::dealinitializefail()
{
    emit initializefailed();
}

void DataBaseInitializer::downloadremoteinfo()
{
    QUrl downloadurl(QString(INFOURL) + "/info.json");
    Downloader *infodownloader = new Downloader(downloadurl, runtimedir + "/cache/info.json");
    QObject::connect(infodownloader, &Downloader::downloadfinished, this, &DataBaseInitializer::readremoteinfo);
    QObject::connect(infodownloader, &Downloader::downloadfailed, this, &DataBaseInitializer::dealinitializefail);
    downloadmanager->adddownloader(infodownloader);
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
    for (int i = 0; i < databaseinfos.count(); i++)
    {
        QJsonObject databaseinfo = databaseinfos.at(i).toObject();
        QString username = databaseinfo.value("name").toString();
        QString token = databaseinfo.value("access_token").toString();
        database->usertokens.insert(username, token);
        QJsonArray reponames = databaseinfo.value("reponames").toArray();
        for (int j = 0; j < reponames.count(); j++)
        {
            QString reponame = reponames.at(j).toString();
            database->usernames.insert(reponame, username);
        }
    }
}

void DataBaseInitializer::parsetips()
{
    QJsonArray tipinfos = database->remoteroot.value("tips").toArray();
    for (int i = 0; i < tipinfos.count(); i++)
        database->tips.append(tipinfos.at(i).toString());
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