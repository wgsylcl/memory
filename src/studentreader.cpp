#include "studentreader.h"

// 读取顺序：学号、名字、生日、电话号码、QQ号、微信号

StudentReader::StudentReader(QObject *parent)
    : QObject{parent}, file("./data/student.csv")
{
}

Q_INVOKABLE int StudentReader::readdata(void)
{
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    QString csvdata = stream.readAll();
    file.close();
    QList<QString> csvstrs = csvdata.split('\n');
    datas.clear();
    csvstrs.pop_back();
    for (QString &csvstr : csvstrs)
    {
        QVector<QString> stu = csvstr.split(',');
        QVariantMap map;
        map.insert("id",stu.at(0));
        map.insert("name",stu.at(1));
        map.insert("birthday",stu.at(2));
        map.insert("profile",stu.at(3));
        datas.push_back(map);
    }
    return datas.size();
}

Q_INVOKABLE QVariantMap StudentReader::at(int i)
{
    return datas.at(i);
}
