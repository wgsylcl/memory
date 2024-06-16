#ifndef TEACHERFILEREADER_H
#define TEACHERFILEREADER_H

#include "base.h"

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
    Q_INVOKABLE QList<QUrl> getpicpaths(void);

private:
    QString name;
    QString subject;
    QStringList petphrase;
    QStringList reviews;
    QList<QUrl> picpaths;
    QFile file;
};

#endif // TEACHERFILEREADER_H
