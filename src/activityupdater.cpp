#include "activityupdater.h"

ActivityUpdater *ActivityUpdater::_instacnce = nullptr;

ActivityUpdater::ActivityUpdater(QObject *parent)
    : QObject{parent}
{
    _instacnce = this;
}

void ActivityUpdater::startupdate(QString activityname)
{
    QMutexLocker locker(&lock);
    updatinglist.insert(activityname);
    QDir activitydir(runtimedir + "/data/activities/" + activityname);
    if (!activitydir.exists())
        activitydir.mkdir(runtimedir + "/data/activities/" + activityname);
    QStringList reponames = database->getactivityreponames(activityname);
    for (QString reponame : reponames)
    {
        FilelistReader *filelistreader = new FilelistReader(reponame);
        QObject::connect(filelistreader, FilelistReader::receivefilelistfinished, this, [activityname, reponame, this](QStringList filelist)
                         { this->updaterepo(activityname, reponame, filelist); });
        repocount[activityname]++;
        filelistreader->startreadfilelist();
    }
}

void ActivityUpdater::updaterepo(const QString activityname, const QString reponame, const QStringList filelist)
{
    const QString activitypath = runtimedir + "/data/activities/" + activityname;
    const QStringList localfilelist = memorybase::getfilenamelist(activitypath);
    for (QString filename : filelist)
    {
        QFileInfo fileinfo(filename);
        if (fileinfo.suffix() == "0")
        {
            filename = fileinfo.completeBaseName();
            if (!localfilelist.contains(filename))
            {
                BigFileDownloader *bigfiledownloader = new BigFileDownloader(reponame, filename, activitypath + "/" + filename);
                QObject::connect(bigfiledownloader, &BigFileDownloader::downloadfinished, this, [activityname, this]()
                                 { this->dealdownloadfinished(activityname); });
                taskcount[activityname]++;
                bigfiledownloader->startdownload();
            }
        }
        else if (!localfilelist.contains(filename))
        {
            Downloader *downloader = new Downloader(database->generaterequesturl(reponame, filename), activitypath + "/" + filename);
            QObject::connect(downloader, &Downloader::downloadfinished, this, [activityname, this]()
                             { this->dealdownloadfinished(activityname); });
            taskcount[activityname]++;
            downloadmanager->adddownloader(downloader);
        }
    }
    if (--repocount[activityname])
        return;
    if (taskcount[activityname])
        return;
    database->syncactivityversion(activityname);
    updatinglist.remove(activityname);
    emit updatefinished(activityname);
}

void ActivityUpdater::dealdownloadfinished(QString activityname)
{
    QMutexLocker locker(&lock);
    if (--taskcount[activityname])
        return;
    database->syncactivityversion(activityname);
    updatinglist.remove(activityname);
    emit updatefinished(activityname);
}

ActivityUpdater *ActivityUpdater::instance()
{
    return _instacnce;
}

Q_INVOKABLE bool ActivityUpdater::is_updating(QString activityname)
{
    return updatinglist.contains(activityname);
}
