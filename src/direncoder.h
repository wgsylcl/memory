#ifndef DIRENCODER_H
#define DIRENCODER_H

#include <QObject>
#include "encodethread.h"

class Direncoder : public QObject
{
    Q_OBJECT
public:
    explicit Direncoder(QObject *parent = nullptr);
    Q_INVOKABLE void encodedir(QString);
signals:
};

#endif // DIRENCODER_H
