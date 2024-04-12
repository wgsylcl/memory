#include "profilereader.h"

ProfileReader::ProfileReader(QObject *parent)
    : QObject{parent}
{}

Q_INVOKABLE void ProfileReader::readfile(int key)
{
    QFile file(QString("./data/students/%1.json").arg(key));
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray jsondata = file.readAll();
    file.close();
    QJsonDocument jsondoc = QJsonDocument::fromJson(jsondata);
    QJsonObject root = jsondoc.object();
    QJsonArray picdatas = root.value("picture").toArray();
    QJsonArray revdatas = root.value("review").toArray();
    // deal with picture :
    this->picpaths.clear();
    for(int i=0;i<picdatas.count();i++)
    {
        picpaths.push_back(QString("file:./data/pictures/")+picdatas.at(i).toString());
    }
    if(picdatas.empty()) picpaths.push_back("file:./data/pictures/nopicture.png");
    // deal with review :
    this->reviews.clear();
    for(int i=0;i<revdatas.count();i++)
    {
        reviews.push_back(revdatas.at(i).toString());
    }
    if(reviews.empty()) reviews.push_back("还没有人给ta留言哦~");
    return;
}

Q_INVOKABLE QStringList ProfileReader::getpicpaths(void)
{
    return this->picpaths;
}

Q_INVOKABLE QStringList ProfileReader::getreviews(void)
{
    return this->reviews;
}
