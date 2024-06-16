#include "database.h"

DataBase *DataBase::_instance = nullptr;

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    _instance = this;
}

DataBase *DataBase::instance()
{
    return _instance;
}

QStringList DataBase::getAllLocalActivitieNames()
{
    QStringList ret;
    for (auto activity : activities)
    {
        if (activity.version != "0.0.0")
            ret.push_back(activity.name);
    }
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
