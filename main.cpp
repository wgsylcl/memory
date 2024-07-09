#include "memoryapplication.h"
#include "app_dump.h"

void logmessagehander(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex logmessagelock;
    QMutexLocker locker(&logmessagelock);

    const char *function = context.function ? context.function : "";

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
    QString OutMsg = time + QString(" %1 :").arg(function) + msg + "\n";

    static QFile *logfile = nullptr;
    if (!logfile)
    {
        QDir logdir(runtimedir + "/logs");
        if (!logdir.exists())
            logdir.mkpath(runtimedir + "/logs");
        database->logfilename = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_v" + VERSION + ".log";
        logfile = new QFile(runtimedir + "/logs/" + database->logfilename);
        logfile->open(QIODevice::WriteOnly | QIODevice::Text);
        QObject::connect(qApp, &QCoreApplication::aboutToQuit, [&]()
                         {
            if(logfile)
            {
                logfile -> close();
                logfile -> deleteLater();
                logfile = nullptr;
            } });
    }

    QTextStream logout(logfile);
    logout << OutMsg;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("wgsylcl");
    QCoreApplication::setApplicationName("memory");

    MemoryApplication app(argc, argv);

    qInstallMessageHandler(logmessagehander);

    qDebug() << "Hello memory!";
    qDebug() << "Run at " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") << "!";

#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
#endif

    return app.exec();
}
