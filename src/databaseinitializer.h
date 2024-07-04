#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include "base.h"
#include "database.h"
#include "downloadmanager.h"

class DataBaseInitializer : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseInitializer(QObject *parent = nullptr);

signals:
    void initializefinished();
    void initializefailed();

public slots:
    void initialize();
    void readremoteinfo();
    void dealinitializefail();

private:
    void downloadremoteinfo();
    void parseprofile();
    void parseactivities();
    void parsepictures();
    void parsedatabaseinfo();
    void readlocalversion();
};

#endif // DATABASEINITIALIZER_H
