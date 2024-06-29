#ifndef MAINTOOL_H
#define MAINTOOL_H

#include "base.h"

class MainTool : public QObject
{
    Q_OBJECT
public:
    explicit MainTool(QObject *parent = nullptr);
    ~MainTool();
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
signals:

private:
};

#endif // MAINTOOL_H
