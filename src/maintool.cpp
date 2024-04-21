#include "maintool.h"

MainTool::MainTool(QObject *parent)
    : QObject{parent}
{}

Q_INVOKABLE QString MainTool::getCurrentApplicationPath(void)
{
    return QCoreApplication::applicationFilePath();
}

void MainTool::setpagetemp(QString newtemp)
{
    m_pagetemp = newtemp;
}

QString MainTool::getpagetemp(void) const
{
    return m_pagetemp;
}
