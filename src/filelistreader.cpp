#include "filelistreader.h"

FilelistReader::FilelistReader(QString reponame,QObject *parent)
    : QObject{parent},reponame(reponame),savepath(runtimedir + "/cache/" + reponame + "_filelist.txt")
{
}

void FilelistReader::startreadfilelist()
{
    Downloader *downloader = new Downloader(memorybase::generaterequesturl(reponame,"filelist.txt"),savepath);
    QObject::connect(downloader,&Downloader::downloadfinished,this,&FilelistReader::receivefilelist);
    downloadmanager->adddownloader(downloader);
}

void FilelistReader::receivefilelist(void)
{
    QFile filelist(savepath);
    filelist.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream qin(&filelist);
    QStringList files;
    while(!qin.atEnd())
        files.append(qin.readLine());
    filelist.close();
    filelist.remove();
    emit receivefilelistfinished(files);
}
