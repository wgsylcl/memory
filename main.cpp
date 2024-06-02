#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QtQml>
#include <QDebug>
#include <QQmlContext>
#include "timelinereader.h"
#include "studentreader.h"
#include "profilereader.h"
#include "teacherfilereader.h"
#include "maintool.h"
#include "activityhelper.h"
#include "FluentPlayer.h"
#include "imageprovider.h"
#include "direncoder.h"
#include "taskloghelper.h"

TasklogManager* TasklogManager::globalinstance = nullptr;

QMutex logmessagelock;

void logmessagehander(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&logmessagelock);

    const char *function = context.function ? context.function : "";

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss:");
    QString OutMsg = time + QString(" %1 ").arg(function) + msg + "\n";

    // 输出信息至文件中
    FILE *f = fopen("log.txt", "a");
    fputs(OutMsg.toStdString().c_str(), f);
    fclose(f);
    f = NULL;
}

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "Template_" + QLocale(locale).name();
        if (translator.load("./i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }

    TasklogManager* tasklogmanager = new TasklogManager(&app);

    qInstallMessageHandler(logmessagehander);

    threadpool->setMaxThreadCount(18);

    qmlRegisterType<TimelineReader>("timelinehelper", 1, 0, "TimelineReader");
    qmlRegisterType<StudentReader>("studenthelper", 1, 0, "StudentReader");
    qmlRegisterType<ProfileReader>("profilehelper", 1, 0, "ProfileReader");
    qmlRegisterType<TeacherFileReader>("teacherfilehelper", 1, 0, "TeacherFileReader");
    qmlRegisterType<FluentPlayer>("mediahelper", 1, 0, "MediaPlayerItem");
    qmlRegisterType<Direncoder>("codehelper", 1, 0, "Direncoder");
    qmlRegisterType<TasklogHelper>("taskloghelper", 1, 0, "TasklogHelper");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("MainTool", new MainTool(&app));
    engine.rootContext()->setContextProperty("ActivityReader", new ActivityHelper(&app));

    engine.addImageProvider("provider", new ImageProvider(&app));

    const QUrl url(QStringLiteral("qrc:/App.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
