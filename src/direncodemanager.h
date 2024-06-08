#ifndef DIRENCODEMANAGER_H
#define DIRENCODEMANAGER_H

#include "direncoder.h"

class Direncodemanager : public QObject
{
    Q_OBJECT
public:
    explicit Direncodemanager(QObject *parent = nullptr);
    Q_INVOKABLE void encodedir(QString);
signals:

private:
    Direncoder *encoder;
    QThread *worker;
};

#endif // DIRENCODEMANAGER_H
