#include "memoryapplication.h"

MemoryApplication::MemoryApplication(int &argc, char *argv[])
    : QGuiApplication{argc, argv}, uiLanguages(QLocale::system().uiLanguages()),
      tasklogmanager(new TasklogManager()), engine(new QQmlApplicationEngine()),
      activityhelper(new ActivityHelper()), maintool(new MainTool()), imageprovider(new ImageProvider()),
      m_database(new DataBase()), filelocker(runtimedir + "/filelock"), m_downloadmanager(new DownloadManager()),
      profilepictureupdater(new ProfilePictureUpdater()), activityupdater(new ActivityUpdater()), uploader(new Uploader()),
      uploadpreviewimageprovider(new UploadPreviewImageProvider())
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    setOrganizationName("wgsylcl");
    setApplicationName("memory");
    checksingle();
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, this, &MemoryApplication::releseresources);
    threadpool->setExpiryTimeout(-1);
    threadpool->setMaxThreadCount(128);
    setupfiles();
    setuptranslator();
    registermodules();
    setupqmlengine();
    startinitalize();
}

MemoryApplication::~MemoryApplication()
{
}

void MemoryApplication::releseresources()
{
    if (databaseinitializethread->isRunning())
        databaseinitializethread->exit();
    if (filelocker.isLocked())
        filelocker.unlock();
    engine->deleteLater();
    activityhelper->deleteLater();
    database->deleteLater();
    downloadmanager->deleteLater();
    maintool->deleteLater();
    tasklogmanager->deleteLater();
    profilepictureupdater->deleteLater();
    activityupdater->deleteLater();
    uploader->deleteLater();
    databaseinitializer->deleteLater();
    databaseinitializethread->deleteLater();
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
    qmlRegisterType<FluentPlayer>("mediahelper", 1, 0, "MediaPlayerItem");
    qmlRegisterType<Direncodemanager>("codehelper", 1, 0, "Direncoder");
    qmlRegisterType<TasklogHelper>("taskloghelper", 1, 0, "TasklogHelper");
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

void MemoryApplication::startinitalize()
{
    databaseinitializethread = new QThread(this);
    databaseinitializer = new DataBaseInitializer();
    databaseinitializer->moveToThread(databaseinitializethread);
    QObject::connect(databaseinitializethread, &QThread::started, databaseinitializer, &DataBaseInitializer::initialize);
    databaseinitializethread->start();
}

void MemoryApplication::setupfiles()
{
    QDir cachedir(runtimedir + "/cache");
    if (!cachedir.exists())
        cachedir.mkpath(runtimedir + "/cache");
}

void MemoryApplication::checksingle()
{
    if (!filelocker.tryLock(1000))
        ::exit(0);
}