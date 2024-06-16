#include "memoryapplication.h"

void logmessagehander(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex logmessagelock;
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
    MemoryApplication app(argc, argv);

    qInstallMessageHandler(logmessagehander);

    return app.exec();
}
