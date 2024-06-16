#include "databaseinitializer.h"

DataBaseInitializer::DataBaseInitializer(QObject *parent)
    : QObject{parent}
{
}

void DataBaseInitializer::receiveremoteinfo()
{
    // QByteArray remoteinfodata;
    // database->remoteroot = QJsonDocument::fromJson(remoteinfodata).object();
    // parseprofile();
    // parsepictures();
    // parseactivities();
}

void DataBaseInitializer::readinfo()
{
    QFile infofile(runtimedir + "/data/info.json");
    infofile.open(QIODevice::ReadOnly);
    database->jsonroot = QJsonDocument::fromJson(infofile.readAll()).object();
}

void DataBaseInitializer::downloadremoteinfo()
{
    Downloader *remoteinfodownloader = new Downloader(QUrl("https://gitee.com/wgsylcl/w2g0e2x1p0e0r1i4m9e4n7t9a2l0s2c1h0o0o1l4m9e4m7o9r1y8d1a8ta/raw/master/info.json.data"),runtimedir + "/cache/remoteinfo.json.data");
    QObject::connect(remoteinfodownloader,&Downloader::downloadfinished,this,&DataBaseInitializer::receiveremoteinfo);
    downloadmanager -> adddownloader(remoteinfodownloader);
}

void DataBaseInitializer::parseprofile()
{
}

void DataBaseInitializer::parseactivities()
{
    QJsonArray jarr = database->jsonroot.value("activities").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jactivitie = jarr.at(i).toObject();
        memorybase::Activity activity;
        activity.name = jactivitie.value("name").toString();
        activity.version = jactivitie.value("version").toString();
        activity.size = jactivitie.value("size").toString();
        QJsonArray downloadurls = jactivitie.value("downloadurl").toArray();
        for (int j = 0; j < downloadurls.count(); j++)
            activity.downloadurl.push_back(downloadurls.at(i).toString());
        activity.fullpath = runtimedir + "/data/activities/" + activity.name + "/";
        database->activities.insert(activity.name, activity);
    }
}

void DataBaseInitializer::parsepictures()
{
}

void DataBaseInitializer::initialize()
{
    readinfo();
    downloadremoteinfo();
}
