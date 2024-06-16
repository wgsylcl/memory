#ifndef STUDENTREADER_H
#define STUDENTREADER_H

// 读取顺序：学号、名字、生日、电话号码、QQ号、微信号

#include "base.h"

class StudentReader : public QObject
{
    Q_OBJECT
public:
    explicit StudentReader(QObject *parent = nullptr);
    Q_INVOKABLE int readdata(void);
    Q_INVOKABLE QVariantMap at(int);
signals:

private:
    QFile file;
    QVector<QVariantMap> datas;
};

#endif // STUDENTREADER_H
