#ifndef STUDENTREADER_H
#define STUDENTREADER_H

#include "base.h"

class StudentReader : public QObject
{
    Q_OBJECT
public:
    explicit StudentReader(QObject *parent = nullptr);
    Q_INVOKABLE QStringList readdata(void);
    Q_INVOKABLE QVariantMap at(QString name);
    Q_INVOKABLE QList<QUrl> getpicpaths(QString name);
    Q_INVOKABLE QStringList getreviews(QString name);
signals:

private:
    QFile file;
    QMap<QString,QVariantMap> datas;
    QMap<QString,QList<QUrl>> picpaths;
    QMap<QString,QStringList> reviews;
};

#endif // STUDENTREADER_H
