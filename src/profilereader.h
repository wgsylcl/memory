#ifndef PROFILEREADER_H
#define PROFILEREADER_H

#include "base.h"

class ProfileReader : public QObject
{
    Q_OBJECT
public:
    explicit ProfileReader(QObject *parent = nullptr);
    ~ProfileReader();
    Q_INVOKABLE void readfile(int key);
    Q_INVOKABLE QList<QUrl> getpicpaths(void);
    Q_INVOKABLE QStringList getreviews(void);
private:
    QList<QUrl> picpaths;
    QStringList reviews;
};

#endif // PROFILEREADER_H
