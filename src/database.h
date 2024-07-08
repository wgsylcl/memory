#ifndef DATABASE_H
#define DATABASE_H

#include "base.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();
    static DataBase *instance();
    QStringList getAllLocalActivitieNames();
    QStringList getAllRemoteActivitieNames();
    memorybase::Activity &getactivity(QString activityname);
    friend class DataBaseInitializer;
    QString getlocalprofilepictureversion(void);
    QString getremoteprofilepictureversion(void);
    QString getlocalactivityversionbyname(QString name);
    QString getremoteactivityversionbyname(QString name);
    QString getprofilereponame(void);
    QStringList getpicturereponames(void);
    QStringList getactivityreponames(QString activityname);
    void synclocalprofilepictureversion(void);
    void syncactivityversion(QString activityname);
    void removelocalactivityversionbyname(QString name);
    QUrl generaterequesturl(QString reponame, QString filepath);
    QString getlatestapplicationversion();
    QStringList gettips();
    QString logfilename;
signals:

private:
    memorybase::Activities activities;
    QString localpictureversion;
    QString remotepictureversion;
    QStringList picturereponames;
    QString localprofileversion;
    QString remoteprofileversion;
    QString profilereponame;
    QString latestapplicationversion;
    QMap<QString, QString> localactivityversions;
    QJsonObject jsonroot;
    QJsonObject remoteroot;
    QMap<QString, QString> usernames, usertokens;
    QStringList tips;
    static DataBase *_instance;
    void saveinfo();
};

#endif // DATABASE_H
