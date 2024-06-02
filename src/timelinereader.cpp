#include "timelinereader.h"

TimelineReader::TimelineReader(QObject *parent)
    : QObject{parent},jsonfile(QCoreApplication::applicationDirPath()+"/data/timelinedata.json")
{}

Q_INVOKABLE int TimelineReader::readdata(void)
{
    QByteArray jsondata;
    if(jsonfile.open(QIODevice::ReadOnly))
    {
        jsondata = jsonfile.readAll();
        jsonfile.close();
        datas.clear();
    }
    QJsonDocument jsondoc = QJsonDocument::fromJson(jsondata);
    QJsonObject root = jsondoc.object();
    QJsonValue jdatas = root.value("data");
    QJsonArray arr = jdatas.toArray();
    for(int i=0;i<arr.count();i++)
    {
        QJsonObject element = arr.at(i).toObject();
        QVariantMap map;
        map["lable"] = element.value("lable");
        map["text"] = element.value("text");
        datas.push_back(map);
    }
    return datas.size();
}

Q_INVOKABLE QVariantMap TimelineReader::at(int i)
{
    return datas.at(i);
}
