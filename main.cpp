#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include "timelinereader.h"
#include "studentreader.h"
#include "profilereader.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Template_" + QLocale(locale).name();
        if (translator.load("./i18n/"+ baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    qmlRegisterType<TimelineReader>("timelinehelper", 1, 0, "TimelineReader");
    qmlRegisterType<StudentReader>("studenthelper", 1, 0, "StudentReader");
    qmlRegisterType<ProfileReader>("profilehelper", 1, 0, "ProfileReader");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
