#ifndef MAINTOOL_H
#define MAINTOOL_H

#include <QObject>
#include <QCoreApplication>
#include <QString>
#include <QUrl>
#include <QFileInfo>

class MainTool : public QObject
{
    Q_OBJECT
public:
    explicit MainTool(QObject *parent = nullptr);
    Q_INVOKABLE QString getCurrentApplicationPath(void);
    Q_INVOKABLE bool isVedio(QUrl url);
signals:

private:
};

#endif // MAINTOOL_H
