#ifndef PACKER_H
#define PACKER_H

#include "base.h"

class Packer : public QThread
{
    Q_OBJECT
public:
    explicit Packer(QString packdir,QString savepath,QObject *parent = nullptr);
    void run();
signals:
    void packupfinished();

private:
    QString packdir;
    QZipWriter *zipwriter;
    QString savepath;
};

#endif // PACKER_H
