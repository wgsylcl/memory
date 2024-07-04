#include "profilepictureupdater.h"

ProfilePictureUpdater *ProfilePictureUpdater::_instance = nullptr;

ProfilePictureUpdater::ProfilePictureUpdater(QObject *parent)
    : QObject{parent}, taskcount(0), updating(false)
{
    _instance = this;
}

void ProfilePictureUpdater::run()
{
    QMutexLocker locker(&lock);
    updating = true;
    updatefail = false;
    taskcount = 0;
    FilelistReader *profilelistreader = new FilelistReader(database->getprofilereponame());
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfinished, this, &ProfilePictureUpdater::updateprofile);
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfinished, profilelistreader, &QObject::deleteLater);
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfailed, this, &ProfilePictureUpdater::dealdownloadfailed);
    QObject::connect(profilelistreader, &FilelistReader::receivefilelistfailed, profilelistreader, &QObject::deleteLater);
    profilelistreader->startreadfilelist();
    QStringList picturereponames = database->getpicturereponames();
    for (QString picturereponame : picturereponames)
    {
        FilelistReader *picturelistreader = new FilelistReader(picturereponame);
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfinished, [picturereponame, this](QStringList filelist)
                         { this->updatepicture(picturereponame, filelist); });
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfinished, picturelistreader, &QObject::deleteLater);
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfailed, this, &ProfilePictureUpdater::dealdownloadfailed);
        QObject::connect(picturelistreader, &FilelistReader::receivefilelistfailed, picturelistreader, &QObject::deleteLater);
        picturelistreader->startreadfilelist();
    }
}

ProfilePictureUpdater *ProfilePictureUpdater::instance(void)
{
    return _instance;
}

void ProfilePictureUpdater::updateprofile(QStringList filelist)
{
    for (QString filename : filelist)
    {
        Downloader *downloader = new Downloader(database->generaterequesturl(database->getprofilereponame(), filename), runtimedir + "/data/students/" + filename);
        QObject::connect(downloader, &Downloader::downloadfinished, this, &ProfilePictureUpdater::dealdownloadfinished);
        QObject::connect(downloader, &Downloader::downloadfailed, this, &ProfilePictureUpdater::dealdownloadfailed);
        downloadmanager->adddownloader(downloader);
        taskcount++;
    }
}

void ProfilePictureUpdater::updatepicture(QString reponame, const QStringList filelist)
{
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
                QObject::connect(bigfiledownloader, &BigFileDownloader::downloadfinished, this, &ProfilePictureUpdater::dealdownloadfinished);
                QObject::connect(bigfiledownloader,&BigFileDownloader::downloadfailed,this,&ProfilePictureUpdater::dealdownloadfailed);
                bigfiledownloader->startdownload();
                taskcount++;
            }
        }
        else if (!localfilelist.contains(filename))
        {
            Downloader *downloader = new Downloader(database->generaterequesturl(reponame, filename), runtimedir + "/data/pictures/" + filename);
            QObject::connect(downloader, &Downloader::downloadfinished, this, &ProfilePictureUpdater::dealdownloadfinished);
            QObject::connect(downloader, &Downloader::downloadfailed, this, &ProfilePictureUpdater::dealdownloadfailed);
            downloadmanager->adddownloader(downloader);
            taskcount++;
        }
    }
}

void ProfilePictureUpdater::dealdownloadfinished()
{
    QMutexLocker locker(&lock);
    if (--taskcount)
        return;
    updating = false;
    database->synclocalprofilepictureversion();
    emit updatefinished();
}

void ProfilePictureUpdater::dealdownloadfailed()
{
    QMutexLocker locker(&lock);
    if(updatefail)
        return;
    updating = false;
    updatefail = true;
    emit updatefailed();
}

Q_INVOKABLE bool ProfilePictureUpdater::is_updating()
{
    return updating;
}
