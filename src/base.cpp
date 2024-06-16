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