#ifndef ENCODETHREAD_H
#define ENCODETHREAD_H

#include <QObject>
#include "tasklogmanager.h"
#include "base.h"
#include "qaesencryption.h"

class EncodeThread : public QObject , public QRunnable
{
    Q_OBJECT
public:
    explicit EncodeThread(QString filepath,QString filelistpath,QString ignorepath,QObject *parent = nullptr);
    void run();
signals:

private:
    QAESEncryption encryption;
    QString filepath;
    QString filelistpath;
    QString ignorepath;
    void writemultifile(QByteArray&);
    void appendfilelist(QString);
    void appendignorefile(QString);
};

#endif // ENCODETHREAD_H
