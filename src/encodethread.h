#ifndef ENCODETHREAD_H
#define ENCODETHREAD_H

#include "base.h"

class EncodeThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit EncodeThread(QString filepath, QObject *parent = nullptr);
    void run();
signals:

private:
    QString filepath;
    void writemultifile(QByteArray &);
};

#endif // ENCODETHREAD_H
