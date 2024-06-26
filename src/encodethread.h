#ifndef ENCODETHREAD_H
#define ENCODETHREAD_H

#include <QObject>
#include "tasklogmanager.h"
#include "base.h"

class EncodeThread : public QObject , public QRunnable
{
    Q_OBJECT
public:
    explicit EncodeThread(QString filepath,QString filelistpath,QString ignorepath,QMutex &appendfilelistlock,QMutex &appendignorefilelock,QObject *parent = nullptr);
    void run();
signals:

private:
    QString filepath;
    QString filelistpath;
    QString ignorepath;
    QMutex &appendfilelistlock;
    QMutex &appendignorefilelock;
    void writemultifile(QByteArray&);
    void appendfilelist(QString);
    void appendignorefile(QString);
};

#endif // ENCODETHREAD_H
