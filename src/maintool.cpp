#include "maintool.h"

MainTool::MainTool(QObject *parent)
    : QObject{parent}
{
}

MainTool::~MainTool()
{
}

Q_INVOKABLE QString MainTool::getCurrentApplicationPath(void)
{
    return QCoreApplication::applicationFilePath();
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
