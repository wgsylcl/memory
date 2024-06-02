#include "taskloghelper.h"

TasklogHelper::TasklogHelper(QObject *parent)
    : QObject{parent}
{
    QObject::connect(TasklogManager::Instance(),&TasklogManager::logupdatesignal,this,&TasklogHelper::addlog);
}

Q_INVOKABLE QString TasklogHelper::getlog(void)
{
    return TasklogManager::Instance() -> getlog();
}

void TasklogHelper::addlog(QString newlog)
{
    QMutexLocker locker(&lock);
    emit logupdatesignal(newlog);
}
