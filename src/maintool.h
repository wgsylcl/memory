#ifndef MAINTOOL_H
#define MAINTOOL_H

#include "database.h"
#include <QCommandLineOption>
#include <QCommandLineParser>

class MainTool : public QObject
{
    Q_OBJECT
public:
    explicit MainTool(QObject *parent = nullptr);
    ~MainTool();
    Q_INVOKABLE bool is_crashmode(void);
    Q_INVOKABLE void savecrashfile(void);
    Q_INVOKABLE QUrl getdownloadurl(void);
    Q_INVOKABLE QString getCurrentApplicationPath(void);
    Q_INVOKABLE bool isvideo(QUrl);
    Q_INVOKABLE QUrl toLocalMediaUrl(QUrl);
    Q_INVOKABLE QString getpassword(void);
    Q_INVOKABLE bool fileexist(QString);
    Q_INVOKABLE bool direxist(QString);
    Q_INVOKABLE QUrl toUrl(QString);
    Q_INVOKABLE QString topath(QUrl);
    Q_INVOKABLE QString trimmed(QString);
    Q_INVOKABLE bool is_empty(QString);
    Q_INVOKABLE void removefile(QString);
    Q_INVOKABLE void creatdir(QString);
    Q_INVOKABLE void restartinitialize(void);
    Q_INVOKABLE void testcrash(void);
    Q_INVOKABLE void restartapplication();
    Q_INVOKABLE void cleanlog();
public slots:
    void dealinitializefinish();
    void dealinitializefail();
signals:
    void initializefinished();
    void initializefailed();
    void requestrestartinitialize();
private:
    bool crashmode;
    QString crashfilename,crashlogname;

};

#endif // MAINTOOL_H
