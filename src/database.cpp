#include "database.h"

DataBase *DataBase::_instance = nullptr;

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    _instance = this;
}

DataBase::~DataBase()
{
    
}

DataBase *DataBase::instance()
{
    return _instance;
}

QStringList DataBase::getAllLocalActivitieNames()
{
    QStringList ret;
    for(auto it = localactivityversions.begin();it != localactivityversions.end();it++)
        ret.append(it.key());
    return ret;
}

memorybase::Activity &DataBase::getactivity(QString activityname)
{
    return activities[activityname];
}

void DataBase::saveinfo()
{
    QJsonDocument jdoc(jsonroot);
    QFile file(runtimedir + "/data/info.json");
    file.open(QIODevice::WriteOnly);
    file.write(jdoc.toJson());
    file.close();
}
