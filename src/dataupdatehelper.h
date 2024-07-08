#ifndef DATAUPDATEHELPER_H
#define DATAUPDATEHELPER_H

#include "database.h"
#include "packer.h"
#include <QVariantMap>

class DataUpdateHelper : public QObject
{
    Q_OBJECT
public:
    explicit DataUpdateHelper(QObject *parent = nullptr);
    Q_INVOKABLE QString getrepopath(QString reponame);
    Q_INVOKABLE void setrepopath(QString reponame, QString repopath);
    Q_INVOKABLE QStringList readuploadfile(QString filepath);
    Q_INVOKABLE void removetask(QString taskid);
    Q_INVOKABLE QVariantMap gettask(QString taskid);
    Q_INVOKABLE QStringList getAllRemoteActivityNames();
    Q_INVOKABLE void createnewactivity(QString activityname, QString repopath);
    Q_INVOKABLE QString gettaskfilebasename();
    Q_INVOKABLE void resetactivitytask(QString taskid, QString newactivityname, QString newactivitypath);
    Q_INVOKABLE void acceptalltask();
    Q_INVOKABLE QStringList getpaths(QString taskid);
    Q_INVOKABLE void removepicture(QString taskid, QUrl picurl);
    Q_INVOKABLE void save_and_pack();
    Q_INVOKABLE QUrl getlocalmediaurl(QUrl url);
signals:

private:
    QSettings setting;
    QString uploadfilebasename, uploaddir;
    QMap<QString, UploadTask> tasks;
    void unpackuploadfile(QString infile, QString outfile);
    static QStringList virtualactivities;
    static Packer *packer;
    void acceptteacherreview(QString sender, QString sendto, QString text);
    void acceptteacherpicture(QString name, QStringList paths);
    void acceptteacherpetphrase(QString name, QString text);
    void acceptstudentreview(QString sender, QString sendto, QString text);
    void acceptstudentpicture(QString name, QStringList paths);
    void acceptstudentprofile(QString name, QString text);
    void acceptstudentbirthday(QString name, QString text);
    void acceptpictureactivity(QString activityname, QStringList paths);
    QByteArray readfile(QString path);
    void writefile(QString path, QByteArray data);
};

#endif // DATAUPDATEHELPER_H
