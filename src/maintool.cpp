#include "maintool.h"

MainTool::MainTool(QObject *parent)
    : QObject{parent}
{
    QCommandLineOption crashop("crashmode");
    QCommandLineOption crashfileop("crashfile");
    crashfileop.setValueName("crashfilepath");
    QCommandLineOption crashlogop("crashlog");
    crashlogop.setValueName("crashlogpath");
    QCommandLineParser parser;
    parser.addOptions({crashop, crashfileop, crashlogop});
    parser.process(*qApp);
    crashmode = parser.isSet(crashop);
    if (parser.isSet(crashfileop))
        crashfilename = parser.value(crashfileop);
    if (parser.isSet(crashlogop))
        crashlogname = parser.value(crashlogop);
}

MainTool::~MainTool()
{
}

Q_INVOKABLE bool MainTool::is_crashmode(void)
{
    return crashmode;
}

Q_INVOKABLE void MainTool::savecrashfile(void)
{
    memorybase::copyfile(runtimedir + "/dmp/" + crashfilename, memorybase::getsystemdownloadpath() + "/" + crashfilename);
    memorybase::copyfile(runtimedir + "/logs/" + crashlogname, memorybase::getsystemdownloadpath() + "/" + crashlogname);
}

Q_INVOKABLE QUrl MainTool::getdownloadurl(void)
{
    return memorybase::toUrl(memorybase::getsystemdownloadpath());
}

Q_INVOKABLE bool MainTool::isvideo(QUrl url)
{
    return memorybase::isvideo(url);
}

Q_INVOKABLE QUrl MainTool::toLocalMediaUrl(QUrl url)
{
    return memorybase::toLocalMediaUrl(url);
}

Q_INVOKABLE QString MainTool::getpassword(void)
{
    return PASSWORD;
}

Q_INVOKABLE bool MainTool::fileexist(QString filepath)
{
    return memorybase::fileexist(filepath);
}

Q_INVOKABLE bool MainTool::direxist(QString dirpath)
{
    return memorybase::direxist(dirpath);
}

Q_INVOKABLE QUrl MainTool::toUrl(QString path)
{
    return memorybase::toUrl(path);
}

Q_INVOKABLE QString MainTool::topath(QUrl url)
{
    return memorybase::topath(url);
}

Q_INVOKABLE QString MainTool::trimmed(QString str)
{
    return str.trimmed();
}

Q_INVOKABLE bool MainTool::is_empty(QString str)
{
    return memorybase::is_empty(str);
}

Q_INVOKABLE void MainTool::removefile(QString path)
{
    memorybase::removefile(path);
}

Q_INVOKABLE void MainTool::creatdir(QString dir)
{
    if (!QDir(dir).exists())
        QDir().mkpath(dir);
}

Q_INVOKABLE void MainTool::restartinitialize(void)
{
    emit requestrestartinitialize();
}

Q_INVOKABLE void MainTool::testcrash(void)
{
    int *p = nullptr;
    *p = 18;
}

Q_INVOKABLE void MainTool::restartapplication()
{
    QProcess::startDetached(QCoreApplication::applicationFilePath());
}

Q_INVOKABLE void MainTool::cleanlog()
{
    QStringList logs = memorybase::getfilenamelist(runtimedir + "/logs");
    for (QString log : logs)
        if (log != database->logfilename)
            QFile(runtimedir + "/logs/" + log).remove();
    QStringList dumps = memorybase::getfilenamelist(runtimedir + "/dmp");
    for (QString dump : dumps)
        QFile(runtimedir + "/dmp/" + dump).remove();
}

Q_INVOKABLE QString MainTool::getlatestapplicationversion()
{
    return database->getlatestapplicationversion();
}

Q_INVOKABLE QString MainTool::getlocalapplicationversion()
{
    return VERSION;
}

Q_INVOKABLE bool MainTool::haveprofilepicture()
{
    return database->getlocalprofilepictureversion() != "0.0.0";
}

void MainTool::dealinitializefail()
{
    emit initializefailed();
}

void MainTool::dealinitializefinish()
{
    emit initializefinished();
}
