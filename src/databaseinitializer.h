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

public slots:
    void initialize();
    void readremoteinfo();

private:
    void downloadremoteinfo();
    void parseprofile();
    void parseactivities();
    void parsepictures();
    void readlocalversion();
};

#endif // DATABASEINITIALIZER_H
