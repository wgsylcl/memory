#include "memoryapplication.h"

MemoryApplication::MemoryApplication(int &argc, char *argv[])
    : QGuiApplication{argc, argv},
      uiLanguages(QLocale::system().uiLanguages()), engine(new QQmlApplicationEngine()),
      activityhelper(new ActivityHelper()), maintool(new MainTool()), imageprovider(new ImageProvider()),
      m_database(new DataBase()), filelocker(runtimedir + "/filelock"), m_downloadmanager(new DownloadManager()),
      profilepictureupdater(new ProfilePictureUpdater()), activityupdater(new ActivityUpdater()), uploader(new Uploader()),
      uploadpreviewimageprovider(new UploadPreviewImageProvider()), databaseinitializer(nullptr), databaseinitializethread(nullptr)
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    checksingle();
    setupfiles();
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, this, &MemoryApplication::releseresources);
    QObject::connect(maintool, &MainTool::requestrestartinitialize, this, &MemoryApplication::startinitialize);
    threadpool->setExpiryTimeout(-1);
    setuptranslator();
    registermodules();
    setupqmlengine();
    startinitialize();
}

MemoryApplication::~MemoryApplication()
{
}

void MemoryApplication::releseresources()
{
    if (databaseinitializer && databaseinitializethread)
    {
        if (databaseinitializethread->isRunning())
            databaseinitializethread->exit();
        databaseinitializer->deleteLater();
    }
    if (filelocker.isLocked())
        filelocker.unlock();
    engine->deleteLater();
    activityhelper->deleteLater();
    database->deleteLater();
    downloadmanager->deleteLater();
    maintool->deleteLater();
    profilepictureupdater->deleteLater();
    activityupdater->deleteLater();
    uploader->deleteLater();
}

void MemoryApplication::setuptranslator()
{
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "Template_" + QLocale(locale).name();
        if (translator.load("./i18n/" + baseName))
        {
            installTranslator(&translator);
            break;
        }
    }
}

void MemoryApplication::registermodules()
{
    qmlRegisterType<TimelineReader>("timelinehelper", 1, 0, "TimelineReader");
    qmlRegisterType<StudentReader>("studenthelper", 1, 0, "StudentReader");
    qmlRegisterType<TeacherFileReader>("teacherfilehelper", 1, 0, "TeacherFileReader");
    qmlRegisterType<Direncodemanager>("codehelper", 1, 0, "Direncoder");
    qmlRegisterType<DataManagerHelper>("datamanagehelper", 1, 0, "DataManageHelper");
    qmlRegisterType<DataUpdateHelper>("dataupdatehelper", 1, 0, "DataUpdateHelper");
}

void MemoryApplication::setupqmlengine()
{
    engine->rootContext()->setContextProperty("MainTool", maintool);
    engine->rootContext()->setContextProperty("uploader", uploader);
    engine->rootContext()->setContextProperty("ActivityReader", activityhelper);
    engine->rootContext()->setContextProperty("profilepictureupdater", profilepictureupdater);
    engine->rootContext()->setContextProperty("activityupdater", activityupdater);
    engine->addImageProvider("provider", imageprovider);
    engine->addImageProvider("uploadpreviewimageprovider", uploadpreviewimageprovider);

    const QUrl url(QStringLiteral("qrc:/App.qml"));
    QObject::connect(
        engine, &QQmlApplicationEngine::objectCreated,
        this, [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine->load(url);
}

void MemoryApplication::startinitialize()
{
    if (maintool->is_crashmode())
        return;
    databaseinitializethread = new QThread();
    databaseinitializer = new DataBaseInitializer();
    databaseinitializer->moveToThread(databaseinitializethread);
    QObject::connect(databaseinitializethread, &QThread::started, databaseinitializer, &DataBaseInitializer::initialize);
    QObject::connect(databaseinitializethread, &QThread::finished, databaseinitializethread, &QObject::deleteLater);
    QObject::connect(databaseinitializer, &DataBaseInitializer::initializefinished, maintool, &MainTool::dealinitializefinish);
    QObject::connect(databaseinitializer, &DataBaseInitializer::initializefailed, maintool, &MainTool::dealinitializefail);
    QObject::connect(databaseinitializer, &DataBaseInitializer::initializefinished, [this](void)
                     {this->databaseinitializer->deleteLater();this->databaseinitializer = nullptr,this->databaseinitializethread = nullptr; });
    QObject::connect(databaseinitializer, &DataBaseInitializer::initializefailed, [this](void)
                     {this->databaseinitializer->deleteLater();this->databaseinitializer = nullptr,this->databaseinitializethread = nullptr; });
    databaseinitializethread->start();
}

void MemoryApplication::setupfiles()
{
    const QString dumpDirPath = runtimedir + "/dmp";
    const QDir dumpDir(dumpDirPath);
    if (!dumpDir.exists())
        dumpDir.mkpath(dumpDirPath);
    const QDir cachedir(runtimedir + "/cache");
    if (!cachedir.exists())
        cachedir.mkpath(runtimedir + "/cache");
    const QString datapath = runtimedir + "/data";
    if(!QDir(datapath).exists())
        QDir().mkpath(datapath);
#ifdef Q_OS_WIN
    SetFileAttributes((LPCWSTR)datapath.unicode(),FILE_ATTRIBUTE_HIDDEN);        // 设置隐藏文件夹
#endif
    if(!QDir(datapath + "/activities").exists())
        QDir().mkpath(datapath + "/activities");
    if(!QDir(datapath + "/pictures").exists())
        QDir().mkpath(datapath + "/pictures");
    if(!QDir(datapath + "/students").exists())
        QDir().mkpath(datapath + "/students");
    QFile versionfile(datapath + "/versions.json");
    if(!versionfile.exists())
    {
        QFile defaultversionfile(":/res/defaultversions.json");
        defaultversionfile.open(QIODevice::ReadOnly | QIODevice::Text);
        versionfile.open(QIODevice::WriteOnly | QIODevice::Text);
        versionfile.write(defaultversionfile.readAll());
        defaultversionfile.close();
        versionfile.close();
    }
}

void MemoryApplication::checksingle()
{
    if (maintool->is_crashmode())
        return;
    if (!filelocker.tryLock(1000))
        ::exit(0);
}