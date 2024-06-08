#ifndef DIRENCODER_H
#define DIRENCODER_H

#include <QObject>
#include "encodethread.h"

class Direncoder : public QObject
{
    Q_OBJECT
public:
    explicit Direncoder(QString path,QObject *parent = nullptr);
public slots:
    void encodedir(QString path);
    void startencodedir();
signals:

private:
    QString tpath;
};

#endif // DIRENCODER_H
