#include "activityhelper.h"

ActivityHelper::ActivityHelper(QObject *parent)
    : QObject{parent},file(QCoreApplication::applicationDirPath()+"/data/activities/info.json")
{
    file.open(QIODevice::ReadOnly);
    QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray jarr = jdoc.object().value("activities").toArray();
    for(int i=0;i<jarr.count();i++) {
        QJsonObject jactivitie = jarr.at(i).toObject();
        Activity activity;
        activity.name = jactivitie.value("name").toString();
        activity.version = jactivitie.value("version").toString();
        activity.size = jactivitie.value("size").toString();
        activity.downloadurl = jactivitie.value("downloadurl").toString();
        activity.fullpath = QCoreApplication::applicationDirPath() + "/data/activities/" + activity.name + "/";
        activities.insert(activity.name,activity);
    }
}

Q_INVOKABLE QStringList ActivityHelper::getAllActivities(void)
{
    QStringList ret;
    for(auto activity:activities) {
        ret.push_back(activity.name);
    }
    return ret;
}

Q_INVOKABLE int ActivityHelper::readAllMedia(QString activityname)
{
    Activity activity = activities[activityname];
    QDir dirc(activity.fullpath);
    QFileInfoList fileinfos = dirc.entryInfoList(QDir::Files);
    for(QFileInfo fileinfo:fileinfos) {
        mediapaths.push_back("file:"+fileinfo.absoluteFilePath());
    }
    return mediapaths.size();
}

Q_INVOKABLE QUrl ActivityHelper::getMediaPath(int index)
{
    return mediapaths.at(index);
}
