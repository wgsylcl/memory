#include "studentreader.h"

StudentReader::StudentReader(QObject *parent)
    : QObject{parent}, file(runtimedir + "/data/students/student.json")
{

}

Q_INVOKABLE QStringList StudentReader::readdata(void)
{
    file.open(QIODevice::ReadOnly);
    QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray students = jdoc.object().value("datas").toArray();
    datas.clear();
    picpaths.clear();
    reviews.clear();
    QStringList ret;
    for (int i = 0; i < students.count(); i++)
    {
        QJsonObject studata = students.at(i).toObject();
        // deal with basic datas:
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
        datas.insert(name,map);
        ret.append(name);
        // deal with pictures:
        QList<QUrl> picurls;
        QJsonArray jpicarr = studata.value("picture").toArray();
        for(int j=0;j<jpicarr.count();j++)
            picurls.append("image://provider/" + jpicarr.at(j).toString());
        if(picurls.isEmpty())
            picurls.append(QUrl("image://provider/nopicture.png"));
        picpaths.insert(name,picurls);
        // deal with reviews:
        QStringList review;
        QJsonArray jreviewarr = studata.value("review").toArray();
        for(int j=0;j<jreviewarr.count();j++)
            review.append(jreviewarr.at(j).toString());
        if(review.isEmpty())
            review.append("还没有人给ta留言哦~");
        reviews.insert(name,review);
    }
    return ret;
}

Q_INVOKABLE QVariantMap StudentReader::at(QString name)
{
    return datas.value(name);
}

Q_INVOKABLE QList<QUrl> StudentReader::getpicpaths(QString name)
{
    return picpaths[name];
}

Q_INVOKABLE QStringList StudentReader::getreviews(QString name)
{
    return reviews[name];
}
