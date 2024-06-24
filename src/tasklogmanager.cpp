#include "tasklogmanager.h"

TasklogManager *TasklogManager::globalinstance = nullptr;

TasklogManager::TasklogManager(QObject *parent)
    : QObject{parent}
{
    if(!globalinstance)
    globalinstance = this;
}

TasklogManager::~TasklogManager()
{
    
}

TasklogManager *TasklogManager::Instance(void)
{
    return globalinstance;
}

void TasklogManager::addlog(QString newlog)
{
    QMutexLocker locker(&lock);
    log += "\n";
    log += newlog;
    emit logupdatesignal(newlog + "\n");
}

QString TasklogManager::getlog(void)
{
    return log;
}
