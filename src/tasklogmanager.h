#ifndef TASKLOGMANAGER_H
#define TASKLOGMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include "base.h"

class TasklogManager : public QObject
{
    Q_OBJECT
public:
    explicit TasklogManager(QObject *parent = nullptr);
    static TasklogManager* Instance(void);
    void addlog(QString);
    QString getlog(void);
signals:
    void logupdatesignal(QString);
private:
    static TasklogManager* globalinstance;
    QMutex lock;
    QString log;
};

#endif // TASKLOGMANAGER_H
