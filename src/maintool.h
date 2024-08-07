#ifndef MAINTOOL_H
#define MAINTOOL_H

#include "downloadmanager.h"
#include <QCommandLineOption>
#include <QCommandLineParser>

class MainTool : public QObject
{
    Q_OBJECT
public:
    explicit MainTool(QObject *parent = nullptr);
    ~MainTool();
    Q_INVOKABLE QString gettip(void);
    Q_INVOKABLE QString getterms(void);
    Q_INVOKABLE QString getpreviews(void);
    Q_INVOKABLE bool is_crashmode(void);
    Q_INVOKABLE bool is_maintaining(void);
    Q_INVOKABLE void savecrashfile(void);
    Q_INVOKABLE QUrl getdownloadurl(void);
    Q_INVOKABLE bool isvideo(QUrl url);
    Q_INVOKABLE QUrl toLocalMediaUrl(QUrl url);
    Q_INVOKABLE QString getpassword(void);
    Q_INVOKABLE bool fileexist(QString filepath);
    Q_INVOKABLE bool direxist(QString dirpath);
    Q_INVOKABLE QUrl toUrl(QString path);
    Q_INVOKABLE QString topath(QUrl url);
    Q_INVOKABLE QString trimmed(QString str);
    Q_INVOKABLE bool is_empty(QString str);
    Q_INVOKABLE void removefile(QString path);
    Q_INVOKABLE void creatdir(QString dir);
    Q_INVOKABLE void restartinitialize(void);
    Q_INVOKABLE void testcrash(void);
    Q_INVOKABLE void restartapplication();
    Q_INVOKABLE void cleanlog();
    Q_INVOKABLE QString getlatestapplicationversion();
    Q_INVOKABLE QString getlocalapplicationversion();
    Q_INVOKABLE bool haveprofilepicture();
    Q_INVOKABLE void downloadlatestapp();
    Q_INVOKABLE bool is_downloadinglatestapp();
public slots:
    void dealinitializefinish();
    void dealinitializefail();
signals:
    void initializefinished();
    void initializefailed();
    void requestrestartinitialize();
    void downloadlatestappfinished();
    void downloadlatestappfailed();

private:
    bool crashmode,isdownloadinglatestapp;
    QString crashfilename, crashlogname;
    QString terms,previews;
};

#endif // MAINTOOL_H
