#include "profilepictureupdater.h"

ProfilePictureUpdater* ProfilePictureUpdater::_instance = nullptr;

ProfilePictureUpdater::ProfilePictureUpdater(QObject *parent)
    : QObject{parent},taskcount(0),updating(false)
{
    _instance = this;
}

void ProfilePictureUpdater::run()
{
    QMutexLocker locker(&lock);
    updating = true;
    FilelistReader *profilelistreader = new FilelistReader(database->getprofilereponame());
    QObject::connect(profilelistreader,&FilelistReader::receivefilelistfinished,this,&ProfilePictureUpdater::updateprofile);
    QObject::connect(profilelistreader,&FilelistReader::receivefilelistfinished,profilelistreader,&QObject::deleteLater);
    profilelistreader->startreadfilelist();
    QStringList picturereponames = database->getpicturereponames();
    for(QString picturereponame:picturereponames)
    {
        FilelistReader *picturelistreader = new FilelistReader(picturereponame);
        QObject::connect(picturelistreader,&FilelistReader::receivefilelistfinished,this,[picturereponame,this](QStringList filelist){this->updatepicture(picturereponame,filelist);});
        QObject::connect(picturelistreader,&FilelistReader::receivefilelistfinished,picturelistreader,&QObject::deleteLater);
        picturelistreader->startreadfilelist();
    }
}

ProfilePictureUpdater *ProfilePictureUpdater::instance(void)
{
    return _instance;
}

void ProfilePictureUpdater::updateprofile(QStringList filelist)
{
    for(QString filename:filelist)
    {
        Downloader *downloader = new Downloader(memorybase::generaterequesturl(database->getprofilereponame(),filename),runtimedir + "/data/students/" + filename);
        QObject::connect(downloader,&Downloader::downloadfinished,this,&ProfilePictureUpdater::dealdownloadfinished);
        downloadmanager->adddownloader(downloader);
        taskcount++;
    }
}

void ProfilePictureUpdater::updatepicture(QString reponame,const QStringList filelist)
{
    const static QStringList localfilelist = memorybase::getfilenamelist(runtimedir + "/data/pictures");
    for(QString filename:filelist)
    {
        QFileInfo fileinfo(filename);
        if(fileinfo.suffix() == "0")
        {
            filename = fileinfo.completeBaseName();
            if(!localfilelist.contains(filename))
            {
                BigFileDownloader *bigfiledownloader = new BigFileDownloader(reponame,filename,runtimedir + "/data/pictures/" + filename);
                QObject::connect(bigfiledownloader,&BigFileDownloader::downloadfinished,bigfiledownloader,&QObject::deleteLater);
                QObject::connect(bigfiledownloader,&BigFileDownloader::downloadfinished,this,&ProfilePictureUpdater::dealdownloadfinished);
                bigfiledownloader->startdownload();
                taskcount++;
            }
        }
        else if(!localfilelist.contains(filename))
        {
            Downloader *downloader = new Downloader(memorybase::generaterequesturl(reponame,filename),runtimedir + "/data/pictures/" + filename);
            QObject::connect(downloader,&Downloader::downloadfinished,this,&ProfilePictureUpdater::dealdownloadfinished);
            downloadmanager->adddownloader(downloader);
            taskcount++;
        }
    }
}

void ProfilePictureUpdater::dealdownloadfinished()
{
    QMutexLocker locker(&lock);
    if(--taskcount) return;
    updating = false;
    database -> synclocalprofilepictureversion();
    emit updatefinished();
}

Q_INVOKABLE bool ProfilePictureUpdater::is_updating()
{
    return updating;
}
