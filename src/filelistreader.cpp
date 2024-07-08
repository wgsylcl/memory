#include "filelistreader.h"

FilelistReader::FilelistReader(QString reponame, QObject *parent)
    : QObject{parent}, reponame(reponame), savepath(runtimedir + "/cache/" + reponame + "_filelist.json")
{
}

void FilelistReader::startreadfilelist()
{
    Downloader *downloader = new Downloader(database->generatefilelistrequesturl(reponame), savepath);
    QObject::connect(downloader, &Downloader::downloadfinished, this, &FilelistReader::receivefilelist);
    QObject::connect(downloader, &Downloader::downloadfailed, this, &FilelistReader::receivefail);
    downloadmanager->adddownloader(downloader);
}

void FilelistReader::receivefail(void)
{
    emit receivefilelistfailed();
}

void FilelistReader::receivefilelist(void)
{
    QFile filelist(savepath);
    filelist.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonArray jarr = QJsonDocument::fromJson(filelist.readAll()).array();
    filelist.close();
    filelist.remove();
    QStringList files;
    for(int i=0;i<jarr.count();i++)
    {
        QString filename(jarr.at(i).toObject().value("path").toString());
        if(memorybase::getfilesuffix(filename) == "json" || memorybase::getfilesuffix(filename) == "0" || memorybase::ismedia(filename))
            files.append(filename);
    }
    emit receivefilelistfinished(files);
}
