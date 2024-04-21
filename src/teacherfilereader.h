#ifndef TEACHERFILEREADER_H
#define TEACHERFILEREADER_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QCoreApplication>

class TeacherFileReader : public QObject
{
    Q_OBJECT
public:
    explicit TeacherFileReader(QObject *parent = nullptr);
    Q_INVOKABLE void readdata(int key);
    Q_INVOKABLE QString getname(void);
    Q_INVOKABLE QString getsubject(void);
    Q_INVOKABLE QStringList getpetphrase(void);
    Q_INVOKABLE QStringList getreviews(void);
    Q_INVOKABLE QStringList getpicpaths(void);

private:
    QString name;
    QString subject;
    QStringList petphrase;
    QStringList reviews;
    QStringList picpaths;
    QFile file;
};

#endif // TEACHERFILEREADER_H
