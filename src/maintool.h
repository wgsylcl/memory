#ifndef MAINTOOL_H
#define MAINTOOL_H

#include <QObject>
#include <QCoreApplication>

class MainTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pagetemp READ getpagetemp WRITE setpagetemp)
public:
    explicit MainTool(QObject *parent = nullptr);
    Q_INVOKABLE QString getCurrentApplicationPath(void);
    void setpagetemp(QString newtemp);
    QString getpagetemp(void) const;
signals:

private:
    QString m_pagetemp;
};

#endif // MAINTOOL_H
