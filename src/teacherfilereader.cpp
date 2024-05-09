#include "teacherfilereader.h"

TeacherFileReader::TeacherFileReader(QObject *parent)
    : QObject{parent},file(QCoreApplication::applicationDirPath()+"/data/teachers.json")
{}

Q_INVOKABLE void TeacherFileReader::readdata(int key)
{
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray jsondata = file.readAll();
    file.close();
    QJsonDocument jsondoc = QJsonDocument::fromJson(jsondata);
    QJsonObject root = jsondoc.object();
    QJsonObject teacher = root.value("datas").toArray().at(key).toObject();
    QJsonArray picdatas = teacher.value("pictures").toArray();
    QJsonArray revdatas = teacher.value("reviews").toArray();
    QJsonArray petphrasedatas = teacher.value("petphrase").toArray();
    // deal with picture :
    this->picpaths.clear();
    for(int i=0;i<picdatas.count();i++)
    {
        picpaths.push_back(QUrl(QString("file:"+QCoreApplication::applicationDirPath()+"/data/pictures/")+picdatas.at(i).toString()));
    }
    if(picpaths.empty()) picpaths.push_back(QUrl("file:"+QCoreApplication::applicationDirPath()+"/data/pictures/nopicture.png"));
    // deal with review :
    this->reviews.clear();
    for(int i=0;i<revdatas.count();i++)
    {
        reviews.push_back(revdatas.at(i).toString());
    }
    if(reviews.empty()) reviews.push_back("还没有人给ta留言哦~");
    // deal with petphrase :
    this->petphrase.clear();
    for(int i=0;i<petphrasedatas.count();i++)
    {
        petphrase.push_back(petphrasedatas.at(i).toString());
    }
    if(petphrase.empty()) petphrase.push_back("ta的口头禅还没收集到噢~");
    // deal with name and subject :
    name = teacher.value("name").toString();
    subject = teacher.value("subject").toString();
    return;
}

Q_INVOKABLE QString TeacherFileReader::getname(void)
{
    return this->name;
}

Q_INVOKABLE QString TeacherFileReader::getsubject(void)
{
    return this->subject;
}

Q_INVOKABLE QStringList TeacherFileReader::getpetphrase(void)
{
    return this->petphrase;
}

Q_INVOKABLE QStringList TeacherFileReader::getreviews(void)
{
    return this->reviews;
}

Q_INVOKABLE QList<QUrl> TeacherFileReader::getpicpaths(void)
{
    return this->picpaths;
}
