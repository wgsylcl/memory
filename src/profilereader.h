#ifndef PROFILEREADER_H
#define PROFILEREADER_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>

class ProfileReader : public QObject
{
    Q_OBJECT
public:
    explicit ProfileReader(QObject *parent = nullptr);
    Q_INVOKABLE void readfile(int key);
    Q_INVOKABLE QStringList getpicpaths(void);
    Q_INVOKABLE QStringList getreviews(void);
private:
    QStringList picpaths;
    QStringList reviews;
};

#endif // PROFILEREADER_H
