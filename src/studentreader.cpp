#include "studentreader.h"

StudentReader::StudentReader(QObject *parent)
    : QObject{parent}, file(runtimedir + "/data/student.json")
{
}

Q_INVOKABLE int StudentReader::readdata(void)
{
    file.open(QIODevice::ReadOnly);
    QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray students = jdoc.object().value("datas").toArray();
    datas.clear();
    for (int i = 0; i < students.count(); i++)
    {
        QJsonObject studata = students.at(i).toObject();
        QVariantMap map;
        QString id = studata.value("id").toString();
        QString name = studata.value("name").toString();
        QString birthday = studata.value("birthday").toString();
        QString profile = studata.value("profile").toString();
        QString sign = studata.value("sign").toString();
        map.insert("id", id);
        map.insert("name", name);
        map.insert("birthday", birthday.isEmpty() ? tr("ta还没提供生日噢~") : birthday);
        map.insert("profile", profile.isEmpty() ? tr("ta还没来个简单的自我介绍噢~") : profile);
        map.insert("sign",QUrl(QString("image://provider/") + (sign.isEmpty() ? "nopicture.png" : sign)));
        datas.push_back(map);
    }
    return datas.size();
}

Q_INVOKABLE QVariantMap StudentReader::at(int i)
{
    return datas.at(i);
}
