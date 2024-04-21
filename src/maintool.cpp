#include "maintool.h"

MainTool::MainTool(QObject *parent)
    : QObject{parent}
{}

Q_INVOKABLE QString MainTool::getCurrentApplicationPath(void)
{
    return QCoreApplication::applicationFilePath();
}
