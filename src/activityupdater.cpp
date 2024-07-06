#include "activityupdater.h"

ActivityUpdater *ActivityUpdater::_instacnce = nullptr;

ActivityUpdater::ActivityUpdater(QObject *parent)
    : QObject{parent}
{
    _instacnce = this;
}

void ActivityUpdater::startupdate(QString activityname)
{
    updatinglist.insert(activityname);
    int idx = ++nidx[activityname];
    repocount[activityname][idx] = 0;
    taskcount[activityname][idx] = 0;
    updatefail[activityname][idx] = false;
    QDir activitydir(runtimedir + "/data/activities/" + activityname);
    if (!activitydir.exists())
        activitydir.mkpath(runtimedir + "/data/activities/" + activityname);
    QStringList reponames = database->getactivityreponames(activityname);
    for (QString reponame : reponames)
    {
        FilelistReader *filelistreader = new FilelistReader(reponame);
        QObject::connect(filelistreader, FilelistReader::receivefilelistfinished, [activityname, reponame, idx, this](QStringList filelist)
                         { this->updaterepo(activityname, reponame, filelist, idx); });
        QObject::connect(filelistreader, FilelistReader::receivefilelistfailed, [activityname, reponame, idx, this]()
                         { this->dealdownloadfailed(activityname, idx); });
        repocount[activityname][idx]++;
        filelistreader->startreadfilelist();
    }
}

void ActivityUpdater::updaterepo(const QString activityname, const QString reponame, const QStringList filelist, const int idx)
{
    if (updatefail[activityname][idx])
        return;
    if (idx < nidx[activityname])
        return;
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
                QObject::connect(bigfiledownloader, &BigFileDownloader::downloadfinished, [activityname, idx, this]()
                                 { this->dealdownloadfinished(activityname, idx); });
                QObject::connect(bigfiledownloader, &BigFileDownloader::downloadfailed, [activityname, idx, this]()
                                 { this->dealdownloadfailed(activityname, idx); });
                taskcount[activityname][idx]++;
                bigfiledownloader->startdownload();
            }
        }
        else if (!localfilelist.contains(filename))
        {
            Downloader *downloader = new Downloader(database->generaterequesturl(reponame, filename), activitypath + "/" + filename);
            QObject::connect(downloader, &Downloader::downloadfinished, [activityname, idx, this]()
                             { this->dealdownloadfinished(activityname, idx); });
            QObject::connect(downloader, &Downloader::downloadfailed, [activityname, idx, this]()
                             { this->dealdownloadfailed(activityname, idx); });
            taskcount[activityname][idx]++;
            downloadmanager->adddownloader(downloader);
        }
    }
    if (--repocount[activityname][idx])
        return;
    if (taskcount[activityname][idx])
        return;
    database->syncactivityversion(activityname);
    updatinglist.remove(activityname);
    emit updatefinished(activityname);
}

void ActivityUpdater::dealdownloadfinished(QString activityname, int idx)
{
    if (updatefail[activityname][idx])
        return;
    if (--taskcount[activityname][idx])
        return;
    if (idx < nidx[activityname])
        return;
    database->syncactivityversion(activityname);
    updatinglist.remove(activityname);
    emit updatefinished(activityname);
}

void ActivityUpdater::dealdownloadfailed(QString activityname, int idx)
{
    if (updatefail[activityname][idx])
        return;
    if (--taskcount[activityname][idx])
        return;
    if (idx < nidx[activityname])
        return;
    updatefail[activityname][idx] = true;
    updatinglist.remove(activityname);
    emit updatefailed(activityname);
}

ActivityUpdater *ActivityUpdater::instance()
{
    return _instacnce;
}

Q_INVOKABLE bool ActivityUpdater::is_updating(QString activityname)
{
    return updatinglist.contains(activityname);
}
