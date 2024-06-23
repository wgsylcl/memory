#include "activityhelper.h"

ActivityHelper::ActivityHelper(QObject *parent)
    : QObject{parent}
{
}

Q_INVOKABLE QStringList ActivityHelper::getAllActivities(void)
{
    return DataBase::instance() -> getAllLocalActivitieNames();
}

Q_INVOKABLE int ActivityHelper::readAllMedia(QString activityname)
{
    memorybase::Activity activity = DataBase::instance() -> getactivity(activityname);
    QDir dirc(getfullpath(activity));
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

QString ActivityHelper::getfullpath(memorybase::Activity activity)
{
    return runtimedir + "/" + activity.name;
}