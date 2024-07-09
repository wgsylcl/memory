#ifndef COMMANDRUNNER_H
#define COMMANDRUNNER_H

#include "base.h"

class CommandRunner : public QObject
{
    Q_OBJECT
public:
    explicit CommandRunner(QObject *parent = nullptr);
    Q_INVOKABLE QString rungitcode(QString dir);
signals:
};

#endif // COMMANDRUNNER_H
