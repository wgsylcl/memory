#ifndef MEMORYAPPLICATION_H
#define MEMORYAPPLICATION_H

#include "base.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QtQml>
#include <QDebug>
#include <QLockFile>
#include <QQmlContext>
#include "timelinereader.h"
#include "studentreader.h"
#include "profilereader.h"
#include "teacherfilereader.h"
#include "maintool.h"
#include "activityhelper.h"
#include "FluentPlayer.h"
#include "imageprovider.h"
#include "direncodemanager.h"
#include "taskloghelper.h"
#include "databaseinitializer.h"
#include "downloadmanager.h"
#include "datamanagerhelper.h"
#include "profilepictureupdater.h"

class MemoryApplication : public QGuiApplication
{
    Q_OBJECT
public:
    explicit MemoryApplication(int &argc,char *argv[]);
    ~MemoryApplication();
signals:

private:
    QQmlApplicationEngine *engine;
    QTranslator translator;
    const QStringList uiLanguages;
    TasklogManager *tasklogmanager;
    ImageProvider *imageprovider;
    MainTool *maintool;
    ActivityHelper *activityhelper;
    DataBaseInitializer *databaseinitializer;
    QThread *databaseinitializethread;
    DataBase *m_database;
    QLockFile filelocker;
    DownloadManager *m_downloadmanager;
    ProfilePictureUpdater *profilepictureupdater;
    void checksingle();
    void setuptranslator();
    void registermodules();
    void setupqmlengine();
    void startinitalize();
    void setupfiles();
    void releseresources();
};

#endif // MEMORYAPPLICATION_H
