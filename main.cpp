#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QtQml>
#include <QQmlContext>
#include "timelinereader.h"
#include "studentreader.h"
#include "profilereader.h"
#include "teacherfilereader.h"
#include "maintool.h"
#include "activityhelper.h"
#include "FluentPlayer.h"

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

    qmlRegisterType<TimelineReader>("timelinehelper", 1, 0, "TimelineReader");
    qmlRegisterType<StudentReader>("studenthelper", 1, 0, "StudentReader");
    qmlRegisterType<ProfileReader>("profilehelper", 1, 0, "ProfileReader");
    qmlRegisterType<TeacherFileReader>("teacherfilehelper", 1, 0, "TeacherFileReader");
    qmlRegisterType<FluentPlayer>("mediahelper", 1, 0, "MediaPlayerItem");

    QQmlApplicationEngine engine;

    MainTool maintool;
    engine.rootContext()->setContextProperty("MainTool", &maintool);
    ActivityHelper activityreader;
    engine.rootContext()->setContextProperty("ActivityReader",&activityreader);

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
