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
#include <QMutex>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QSettings>
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
    bool ismedia(QUrl);
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
    bool is_empty(QString str);
    QString getdirsize(QDir dir);
    QString getsystemdownloadpath(void);
    void copyfile(QString from, QString to);
    void removefile(QString path);
    QString getfilename(QString path);
    QString getfilebasename(QString filename);
    QString generaterandomqstring(int length = 18);
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

class UploadTasktype : public QObject
{
public:
    UploadTasktype(QObject *parent = nullptr) : QObject{parent} {}
    Q_ENUMS(TaskType)
    enum TaskType
    {
        reviewtostudent,
        reviewtoteacher,
        picturetostudent,
        picturetoteacher,
        petphrasetoteacher,
        timeline,
        picturetoactivity,
        profiletostudent,
        birthdaytostudent
    };
};

typedef UploadTasktype::TaskType UploadTaskType;

struct UploadTask
{
    UploadTaskType type;
    QString sender;
    QString sendto;
    QString text;
    QStringList paths;
};

#endif // BASE_H