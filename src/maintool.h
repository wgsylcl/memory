#ifndef MAINTOOL_H
#define MAINTOOL_H

#include <QObject>
#include <QCoreApplication>

class MainTool : public QObject
{
    Q_OBJECT
public:
    explicit MainTool(QObject *parent = nullptr);
    Q_INVOKABLE QString getCurrentApplicationPath(void);
signals:

private:
};

#endif // MAINTOOL_H
