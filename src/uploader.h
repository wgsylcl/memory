#ifndef UPLOADER_H
#define UPLOADER_H

#include "base.h"
#include "packer.h"

class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QObject *parent = nullptr);
    ~Uploader();
    Q_INVOKABLE void addreviewtostudent(QString sender, QString sendto, QString text);
    Q_INVOKABLE void addreviewtoteacher(QString sender, QString sendto, QString text);
    Q_INVOKABLE void addstudentpicture(QString name, QStringList path);
    Q_INVOKABLE void addteacherpicture(QString name, QStringList path);
    Q_INVOKABLE void addstudentbirthday(QString name, QString date);
    Q_INVOKABLE void addteacherpetphrase(QString name, QString text);
    Q_INVOKABLE void addactivitypicture(QString name, QStringList path);
    Q_INVOKABLE void addstudentprofile(QString name, QString text);
    Q_INVOKABLE void addtimeline(QString time, QString text);
    Q_INVOKABLE int gettaskcount();
    Q_INVOKABLE QStringList getalltaskids();
    Q_INVOKABLE int gettasktype(QString id);
    Q_INVOKABLE QString getsender(QString id);
    Q_INVOKABLE QString getsendto(QString id);
    Q_INVOKABLE QString gettext(QString id);
    Q_INVOKABLE QStringList geturls(QString id);
    Q_INVOKABLE QUrl tourl(QString str);
    Q_INVOKABLE void packup();
    Q_INVOKABLE void removetask(QString id);
    Q_INVOKABLE bool is_packing();
    Q_INVOKABLE QString getlastsender();
    Q_INVOKABLE void removepicture(QString taskid, QUrl url);
    Q_INVOKABLE QUrl getlocalmediaurl(QUrl url);
    bool packing;
    QMap<QString, UploadTask> tasks;
public slots:
    void savefile();
signals:
    void packupfinished(QUrl packfileurl);

private:
    Packer *packer;
    QFile taskfile;
    QMutex lock;
    const QString uploaddir;
    QString lastsender;
    void readtaskfile();
};

#endif // UPLOADER_H
