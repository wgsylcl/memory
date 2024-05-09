#include "maintool.h"

MainTool::MainTool(QObject *parent)
    : QObject{parent}
{}

Q_INVOKABLE QString MainTool::getCurrentApplicationPath(void)
{
    return QCoreApplication::applicationFilePath();
}

Q_INVOKABLE bool MainTool::isVedio(QUrl url)
{
    QFileInfo fileinfo(url.toLocalFile());
    QString suffix = fileinfo.suffix().toLower();
    return suffix == "mp4" || suffix == "mkv";
}
