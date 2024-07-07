#include "activityhelper.h"

ActivityHelper::ActivityHelper(QObject *parent)
    : QObject{parent}
{
}

ActivityHelper::~ActivityHelper()
{
}

Q_INVOKABLE QStringList ActivityHelper::getAllActivities(void)
{
    return DataBase::instance()->getAllLocalActivitieNames();
}

Q_INVOKABLE QStringList ActivityHelper::getAllRemoteActivities(void)
{
    return DataBase::instance()->getAllRemoteActivitieNames();
}

Q_INVOKABLE int ActivityHelper::readAllMedia(QString activityname)
{
    memorybase::Activity activity = DataBase::instance()->getactivity(activityname);
    QFileInfoList fileinfos = memorybase::getfileinfolist(getfullpath(activity));
    mediapaths.clear();
    for (QFileInfo fileinfo : fileinfos)
        if (memorybase::ismedia(fileinfo.absoluteFilePath()))
            mediapaths.push_back(fileinfo.absoluteFilePath());
    for (int t = 0; t < 18380; t++)
    {
        static QRandomGenerator *generator = QRandomGenerator::global();
        int i = generator->bounded(0, mediapaths.size() - 1);
        int j = generator->bounded(0, mediapaths.size() - 1);
        memorybase::swap(mediapaths[i], mediapaths[j]);
    }
    return mediapaths.size();
}

Q_INVOKABLE QUrl ActivityHelper::getMediaPath(int index)
{
    return memorybase::toUrl(mediapaths.at(index % mediapaths.size()));
}

QString ActivityHelper::getfullpath(memorybase::Activity activity)
{
    return runtimedir + "/data/activities/" + activity.name;
}