#ifndef TASKLOGHELPER_H
#define TASKLOGHELPER_H

#include <QObject>
#include "tasklogmanager.h"

class TasklogHelper : public QObject
{
    Q_OBJECT
public:
    explicit TasklogHelper(QObject *parent = nullptr);
    Q_INVOKABLE QString getlog(void);

public slots:
    void addlog(QString);

signals:
    void logupdatesignal(QString);

private:
    QMutex lock;
};

#endif // TASKLOGHELPER_H
