#include "maintool.h"

MainTool::MainTool(QObject *parent)
    : QObject{parent}
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
