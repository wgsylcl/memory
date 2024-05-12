#ifndef MAINTOOL_H
#define MAINTOOL_H

#include "base.h"

class MainTool : public QObject
{
    Q_OBJECT
public:
    explicit MainTool(QObject *parent = nullptr);
    Q_INVOKABLE QString getCurrentApplicationPath(void);
    Q_INVOKABLE bool isvideo(QUrl);
    Q_INVOKABLE QUrl toLocalMediaUrl(QUrl);
signals:

private:
};

#endif // MAINTOOL_H
