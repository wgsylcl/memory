#ifndef TIMELINEREADER_H
#define TIMELINEREADER_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QCoreApplication>

class TimelineReader : public QObject
{
    Q_OBJECT
public:
    explicit TimelineReader(QObject *parent = nullptr);
    Q_INVOKABLE int readdata(void);
    Q_INVOKABLE QVariantMap at(int i);
signals:

private:
    QFile jsonfile;
    QList<QVariantMap> datas;
};

#endif // TIMELINEREADER_H
