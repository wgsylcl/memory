#include "commandrunner.h"

CommandRunner::CommandRunner(QObject *parent)
    : QObject{parent}
{
}

Q_INVOKABLE QString CommandRunner::rungitcode(QString dir)
{
    const QString gitcommand = R"(git pull;git add .;git commit -m "update";git push -u origin "master")";
    QProcess process;
    process.setWorkingDirectory(dir);
    process.start("powershell", {gitcommand});
    process.waitForFinished(18000000);
    return process.readAllStandardOutput();
}
