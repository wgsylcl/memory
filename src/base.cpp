#include "base.h"

bool memorybase::isvideo(QUrl url) {
    QFileInfo fileinfo(url.toLocalFile());
    QString suffix = fileinfo.suffix().toLower();
    return suffix == "mp4" || suffix == "mkv";
}

QUrl memorybase::toLocalMediaUrl(QUrl url) {
    return QUrl("file:" + QCoreApplication::applicationDirPath() + "/data/pictures/" + url.fileName());
}

QUrl memorybase::toLocalMediaUrl(QString filename) {
    return QUrl("file:" + QCoreApplication::applicationDirPath() + "/data/pictures/" + filename);
}