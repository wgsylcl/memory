#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QObject>

class DownloadThread : public QObject
{
    Q_OBJECT
public:
    explicit DownloadThread(QObject *parent = nullptr);

signals:
};

#endif // DOWNLOADTHREAD_H
