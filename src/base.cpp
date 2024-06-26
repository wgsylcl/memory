#include "base.h"

bool memorybase::isvideo(QUrl url)
{
    QString suffix = memorybase::getfilesuffix(url);
    return suffix == "mp4" || suffix == "mkv";
}

QUrl memorybase::toLocalMediaUrl(QUrl url)
{
    return QUrl("file:" + runtimedir + "/data/pictures/" + url.fileName());
}

QUrl memorybase::toLocalMediaUrl(QString filename)
{
    return QUrl("file:" + runtimedir + "/data/pictures/" + filename);
}

bool memorybase::fileexist(QString filepath)
{
    return filepath.size() && QFile(filepath).exists();
}

bool memorybase::direxist(QString dirpath)
{
    return dirpath.size() && QDir(dirpath).exists();
}

QUrl memorybase::toUrl(QString path)
{
    return QUrl::fromLocalFile(path);
}

QString memorybase::topath(QUrl url)
{
    return url.toLocalFile();
}

QString memorybase::getfilesuffix(QString filepath)
{
    QFileInfo fileinfo(filepath);
    return fileinfo.suffix().toLower();
}

QString memorybase::getfilesuffix(QUrl url)
{
    QFileInfo fileinfo(url.toLocalFile());
    return fileinfo.suffix().toLower();
}

QUrl memorybase::generaterequesturl(QString reponame,QString filepath)
{
    return QUrl(QString("https://gitee.com/api/v5/repos/wgsylcl/%1/raw/%2?access_token=%3").arg(reponame,filepath,TOKEN));
}

QStringList memorybase::getfilenamelist(QDir dir)
{
    return dir.entryList(QDir::Files);
}

QFileInfoList memorybase::getfileinfolist(QDir dir)
{
    return dir.entryInfoList(QDir::Files);
}

QString memorybase::getdirsize(QDir dir)
{
    qint64 size = 0;
    QFileInfoList infolist = getfileinfolist(dir);
    for(QFileInfo fileinfo:infolist)
        size += fileinfo.size();
    return to_qstring(size >> 20) + "MB";
}

QString memorybase::to_qstring(int num)
{
    return QString::fromStdString(std::to_string(num));
}

QString memorybase::to_qstring(qint64 num)
{
    return QString::fromStdString(std::to_string(num));
}
