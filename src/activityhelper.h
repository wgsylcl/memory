#ifndef ACTIVITYHELPER_H
#define ACTIVITYHELPER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>

class ActivityHelper : public QObject
{
    Q_OBJECT
public:
    explicit ActivityHelper(QObject *parent = nullptr);
    Q_INVOKABLE QStringList getAllActivities(void);
    Q_INVOKABLE int readAllMedia(QString);
    Q_INVOKABLE QUrl getMediaPath(int index);
signals:

private:
    struct Activity
    {
        QString name;
        QString version;
        QString size;
        QString downloadurl;
        QString fullpath;
    };
    QMap<QString,Activity> activities;
    QStringList mediapaths;
    QFile file;
};

#endif // ACTIVITYHELPER_H
