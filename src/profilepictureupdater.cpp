#include "profilepictureupdater.h"

ProfilePictureUpdater *ProfilePictureUpdater::_instance = nullptr;

ProfilePictureUpdater::ProfilePictureUpdater(QObject *parent)
    : QObject{parent}, nidx(0), updating(false)
{
    _instance = this;
}

void ProfilePictureUpdater::run()
{
    int idx = ++nidx;
    updating = true;
    updatefail[idx] = false;
    taskcount[idx] = 0;
    FilelistReader *profilelistreader = new FilelistReader(database->getprofilereponame());
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfinished, [idx, this](QStringList filelist)
                     { this->updateprofile(filelist, idx); });
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfinished, profilelistreader, &QObject::deleteLater);
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfailed, [idx, this]()
                     { this->dealdownloadfailed(idx); });
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfailed, profilelistreader, &QObject::deleteLater);
    profilelistreader->startreadfilelist();
    QStringList picturereponames = database->getpicturereponames();
    for (QString picturereponame : picturereponames)
    {
        FilelistReader *picturelistreader = new FilelistReader(picturereponame);
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfinished, [picturereponame, idx, this](QStringList filelist)
                         { this->updatepicture(picturereponame, filelist, idx); });
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfinished, picturelistreader, &QObject::deleteLater);
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfailed, [idx, this]()
                         { this->dealdownloadfailed(idx); });
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfailed, picturelistreader, &QObject::deleteLater);
        picturelistreader->startreadfilelist();
    }
}

ProfilePictureUpdater *ProfilePictureUpdater::instance(void)
{
    return _instance;
}

void ProfilePictureUpdater::updateprofile(QStringList filelist, int idx)
{
    if (idx < nidx)
        return;
    for (QString filename : filelist)
    {
        Downloader *downloader = new Downloader(database->generaterequesturl(database->getprofilereponame(), filename), runtimedir + "/data/students/" + filename);
        QObject::connect(downloader, &Downloader::downloadfinished, [idx,this](){this->dealdownloadfinished(idx);});
        QObject::connect(downloader, &Downloader::downloadfailed, [idx,this](){this->dealdownloadfailed(idx);});
        downloadmanager->adddownloader(downloader);
        taskcount[idx]++;
    }
}

void ProfilePictureUpdater::updatepicture(QString reponame, const QStringList filelist, int idx)
{
    if (idx < nidx)
        return;
    const static QStringList localfilelist = memorybase::getfilenamelist(runtimedir + "/data/pictures");
    for (QString filename : filelist)
    {
        QFileInfo fileinfo(filename);
        if (fileinfo.suffix() == "0")
        {
            filename = fileinfo.completeBaseName();
            if (!localfilelist.contains(filename))
            {
                BigFileDownloader *bigfiledownloader = new BigFileDownloader(reponame, filename, runtimedir + "/data/pictures/" + filename);
                QObject::connect(bigfiledownloader, &BigFileDownloader::downloadfinished, bigfiledownloader, &QObject::deleteLater);
                QObject::connect(bigfiledownloader, &BigFileDownloader::downloadfinished, [idx,this](){this->dealdownloadfinished(idx);});
                QObject::connect(bigfiledownloader, &BigFileDownloader::downloadfailed, [idx,this](){this->dealdownloadfailed(idx);});
                bigfiledownloader->startdownload();
                taskcount[idx]++;
            }
        }
        else if (!localfilelist.contains(filename))
        {
            Downloader *downloader = new Downloader(database->generaterequesturl(reponame, filename), runtimedir + "/data/pictures/" + filename);
            QObject::connect(downloader, &Downloader::downloadfinished, [idx,this](){this->dealdownloadfinished(idx);});
            QObject::connect(downloader, &Downloader::downloadfailed, [idx,this](){this->dealdownloadfailed(idx);});
            downloadmanager->adddownloader(downloader);
            taskcount[idx]++;
        }
    }
}

void ProfilePictureUpdater::dealdownloadfinished(int idx)
{
    if (idx < nidx)
        return;
    if (--taskcount[idx])
        return;
    updating = false;
    database->synclocalprofilepictureversion();
    emit updatefinished();
}

void ProfilePictureUpdater::dealdownloadfailed(int idx)
{
    if (idx < nidx)
        return;
    if (updatefail[idx])
        return;
    updating = false;
    updatefail[idx] = true;
    emit updatefailed();
}

Q_INVOKABLE bool ProfilePictureUpdater::is_updating()
{
    return updating;
}
