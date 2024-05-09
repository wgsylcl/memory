#include "activityhelper.h"

ActivityHelper::ActivityHelper(QObject *parent)
    : QObject{parent}, file(QCoreApplication::applicationDirPath() + "/data/activities/info.json")
{
    file.open(QIODevice::ReadOnly);
    QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray jarr = jdoc.object().value("activities").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jactivitie = jarr.at(i).toObject();
        Activity activity;
        activity.name = jactivitie.value("name").toString();
        activity.version = jactivitie.value("version").toString();
        activity.size = jactivitie.value("size").toString();
        activity.downloadurl = jactivitie.value("downloadurl").toString();
        activity.fullpath = QCoreApplication::applicationDirPath() + "/data/activities/" + activity.name + "/";
        activities.insert(activity.name, activity);
    }
}

Q_INVOKABLE QStringList ActivityHelper::getAllActivities(void)
{
    QStringList ret;
    for (auto activity : activities)
    {
        ret.push_back(activity.name);
    }
    return ret;
}

Q_INVOKABLE int ActivityHelper::readAllMedia(QString activityname)
{
    Activity activity = activities[activityname];
    QDir dirc(activity.fullpath);
    QFileInfoList fileinfos = dirc.entryInfoList(QDir::Files);
    mediapaths.clear();
    QList<QUrl> videopaths, imagepaths;
    for (QFileInfo fileinfo : fileinfos)
    {
        QString suffix = fileinfo.suffix().toLower();
        if (suffix == "mp4" || suffix == "mkv")
            videopaths.push_back("file:" + fileinfo.absoluteFilePath());
        else
            imagepaths.push_back("file:" + fileinfo.absoluteFilePath());
    }
    std::default_random_engine rng(static_cast<unsigned int>(std::time(nullptr)));
    std::shuffle(videopaths.begin(), videopaths.end(), rng);
    std::shuffle(imagepaths.begin(), imagepaths.end(), rng);
    int videoindex = 0, imageindex = 0, videosize = videopaths.size(), imagesize = imagepaths.size();
    int rate = qMax(imagesize / qMax(videosize, 1), 1);
    while (videoindex < videosize)
    {
        mediapaths.push_back(videopaths.at(videoindex++));
        for (int i = 0; i < rate; i++)
            mediapaths.push_back(imagepaths.at((imageindex++) % imagesize));
    }
    while (imageindex < imagesize)
        mediapaths.push_back(imagepaths.at(imageindex++));
    return mediapaths.size();
}

Q_INVOKABLE QUrl ActivityHelper::getMediaPath(int index)
{
    return mediapaths.at(index % mediapaths.size());
}
