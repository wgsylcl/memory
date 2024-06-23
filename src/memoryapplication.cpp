#include "memoryapplication.h"

MemoryApplication::MemoryApplication(int &argc, char *argv[])
    : QGuiApplication{argc,argv},uiLanguages(QLocale::system().uiLanguages()),
    tasklogmanager(new TasklogManager(this)),engine(new QQmlApplicationEngine(this)),
    activityhelper(new ActivityHelper(this)),maintool(new MainTool(this)),imageprovider(new ImageProvider(this)),
    m_database(new DataBase(this)),filelocker(runtimedir + "/filelock"),m_downloadmanager(new DownloadManager(this))
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
    // checksingle();
    threadpool -> setExpiryTimeout(-1);
    setupfiles();
    setuptranslator();
    registermodules();
    setupqmlengine();
    startinitalize();
}

MemoryApplication::~MemoryApplication()
{
    if(databaseinitializethread -> isRunning())
        databaseinitializethread -> exit();
    databaseinitializer -> deleteLater();
    if(filelocker.isLocked())
        filelocker.unlock();
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
    qmlRegisterType<ProfileReader>("profilehelper", 1, 0, "ProfileReader");
    qmlRegisterType<TeacherFileReader>("teacherfilehelper", 1, 0, "TeacherFileReader");
    qmlRegisterType<FluentPlayer>("mediahelper", 1, 0, "MediaPlayerItem");
    qmlRegisterType<Direncodemanager>("codehelper", 1, 0, "Direncoder");
    qmlRegisterType<TasklogHelper>("taskloghelper", 1, 0, "TasklogHelper");
}

void MemoryApplication::setupqmlengine()
{
    engine -> rootContext() -> setContextProperty("MainTool", maintool);
    engine -> rootContext() -> setContextProperty("ActivityReader", activityhelper);
    engine -> addImageProvider("provider", imageprovider);

    const QUrl url(QStringLiteral("qrc:/App.qml"));
    QObject::connect(
        engine, &QQmlApplicationEngine::objectCreated,
        this, [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine -> load(url);
}

void MemoryApplication::startinitalize()
{
    databaseinitializethread = new QThread(this);
    databaseinitializer = new DataBaseInitializer();
    databaseinitializer -> moveToThread(databaseinitializethread);
    QObject::connect(databaseinitializethread,&QThread::started,databaseinitializer,&DataBaseInitializer::initialize);
    databaseinitializethread -> start();
}

void MemoryApplication::setupfiles()
{
    QDir cachedir(runtimedir + "/cache");
    if(!cachedir.exists())
        cachedir.mkdir(runtimedir + "/cache");
}

void MemoryApplication::checksingle()
{
    if(!filelocker.tryLock(1000))
        ::exit(0);
}