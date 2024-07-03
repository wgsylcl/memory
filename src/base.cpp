#include "base.h"

bool memorybase::isvideo(QUrl url)
{
    QString suffix = memorybase::getfilesuffix(url);
    return suffix == "mp4" || suffix == "mkv";
}

bool memorybase::ismedia(QUrl url)
{
    QString suffix = memorybase::getfilesuffix(url);
    const QStringList suffixbase = {"mp4","mkv","jpg","jpeg","png","svg"};
    return suffixbase.contains(suffix);
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
    for (QFileInfo fileinfo : infolist)
        size += fileinfo.size();
    return to_qstring(size >> 20) + "MB";
}

QString memorybase::getsystemdownloadpath(void)
{
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}

void memorybase::copyfile(QString from, QString to)
{
    QFileInfo fileinfo(to);
    QDir dir = fileinfo.absoluteDir();
    if(!dir.exists()) dir.mkpath(dir.absolutePath());
    QFile infile(from), outfile(to);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    outfile.write(infile.readAll());
    infile.close(), outfile.close();
}

void memorybase::removefile(QString path)
{
    QFile(path).remove();
}

QString memorybase::getfilename(QString path)
{
    return QFileInfo(path).fileName();
}

QString memorybase::getfilebasename(QString filename)
{
    return QFileInfo(filename).baseName();
}

QString memorybase::generaterandomqstring(int length)
{
    static QRandomGenerator *randomgenerator = QRandomGenerator::global();
    const static QString charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
    QString ret = "";
    for (int i = 0; i < length; i++)
        ret += charset[randomgenerator->bounded(charset.size() - 1)];
    return ret;
}

QString memorybase::to_qstring(int num)
{
    return QString::fromStdString(std::to_string(num));
}

QString memorybase::to_qstring(qint64 num)
{
    return QString::fromStdString(std::to_string(num));
}

bool memorybase::is_empty(QString str)
{
    return str.trimmed().isEmpty();
}
