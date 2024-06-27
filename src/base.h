#ifndef BASE_H
#define BASE_H

#include <QFileInfo>
#include <QObject>
#include <QFile>
#include <QImage>
#include <QUrl>
#include <QString>
#include <QPainter>
#include <QDir>
#include <QCoreApplication>
#include <QThreadPool>
#include <QThread>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QtConcurrent>
#include <private/qzipreader_p.h>
#include <private/qzipwriter_p.h>

#define threadpool QThreadPool::globalInstance()
#define database DataBase::instance()
#define downloadmanager DownloadManager::instance()
#define runtimedir QCoreApplication::applicationDirPath()

const qint64 chunkSize = 99 * 1024 * 1024;

namespace memorybase
{
    bool isvideo(QUrl);
    QUrl toLocalMediaUrl(QUrl);
    QUrl toLocalMediaUrl(QString);
    bool fileexist(QString);
    bool direxist(QString);
    QUrl toUrl(QString);
    QString topath(QUrl);
    QString getfilesuffix(QString);
    QString getfilesuffix(QUrl);
    QStringList getfilenamelist(QDir dir);
    QFileInfoList getfileinfolist(QDir dir);
    QString to_qstring(int num);
    QString to_qstring(qint64 num);
    QString getdirsize(QDir dir);
    struct Activity
    {
        QString name;
        QString localversion = "0.0.0";
        QString remoteversion;
        QString size;
        QStringList reponame;
    };
    using Activities = QMap<QString, memorybase::Activity>;

}
#endif // BASE_H