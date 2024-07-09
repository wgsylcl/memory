#ifndef MEMORYAPPLICATION_H
#define MEMORYAPPLICATION_H

#include "base.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QDebug>
#include <QLockFile>
#include <QQmlContext>
#include "timelinereader.h"
#include "studentreader.h"
#include "teacherfilereader.h"
#include "maintool.h"
#include "activityhelper.h"
#include "imageprovider.h"
#include "direncodemanager.h"
#include "databaseinitializer.h"
#include "downloadmanager.h"
#include "datamanagerhelper.h"
#include "profilepictureupdater.h"
#include "activityupdater.h"
#include "uploader.h"
#include "uploadpreviewimageprovider.h"
#include "dataupdatehelper.h"
#include "commandrunner.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

class MemoryApplication : public QGuiApplication
{
    Q_OBJECT
public:
    explicit MemoryApplication(int &argc, char *argv[]);
    ~MemoryApplication();
public slots:
    void startinitialize();
signals:

private:
    QQmlApplicationEngine *engine;
    const QStringList uiLanguages;
    ImageProvider *imageprovider;
    MainTool *maintool;
    ActivityHelper *activityhelper;
    DataBaseInitializer *databaseinitializer;
    QThread *databaseinitializethread;
    DataBase *m_database;
    QLockFile filelocker;
    DownloadManager *m_downloadmanager;
    ProfilePictureUpdater *profilepictureupdater;
    ActivityUpdater *activityupdater;
    Uploader *uploader;
    UploadPreviewImageProvider *uploadpreviewimageprovider;
    CommandRunner *commandrunner;
    void checksingle();
    void registermodules();
    void setupqmlengine();
    void setupfiles();
    void releseresources();
};

#endif // MEMORYAPPLICATION_H
