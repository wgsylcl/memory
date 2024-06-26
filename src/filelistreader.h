#ifndef FILELISTREADER_H
#define FILELISTREADER_H

#include "downloadmanager.h"

class FilelistReader : public QObject
{
    Q_OBJECT
public:
    explicit FilelistReader(QString reponame,QObject *parent = nullptr);
    void startreadfilelist();

public slots:
    void receivefilelist(void);

signals:
    void receivefilelistfinished(QStringList);

private:
    QString reponame,savepath;
};

#endif // FILELISTREADER_H
