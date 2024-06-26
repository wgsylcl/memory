#ifndef PROFILEPICTUREUPDATER_H
#define PROFILEPICTUREUPDATER_H

#include "filelistreader.h"
#include "database.h"
#include "bigfiledownloader.h"

class ProfilePictureUpdater : public QObject
{
    Q_OBJECT
public:
    explicit ProfilePictureUpdater(QObject *parent = nullptr);
    void run();
    static ProfilePictureUpdater* instance(void);
public slots:
    void updateprofile(QStringList filelist);
    void updatepicture(QString reponame,QStringList filelist);
    void dealdownloadfinished();
    Q_INVOKABLE bool is_updating();
signals:
    void updatefinished();
private:
    QMutex lock;
    int taskcount;
    bool updating;
    static ProfilePictureUpdater *_instance;
};

#endif // PROFILEPICTUREUPDATER_H
