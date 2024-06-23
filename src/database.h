#ifndef DATABASE_H
#define DATABASE_H

#include "base.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    static DataBase* instance();
    QStringList getAllLocalActivitieNames();
    memorybase::Activity& getactivity(QString activityname);
    friend class DataBaseInitializer;

signals:

protected:
    memorybase::Activities activities;
    QString localpictureversion;
    QString remotepictureversion;
    QStringList picturereponames;
    QString localprofileversion;
    QString remoteprofileversion;
    QString profilereponame;
    QMap<QString,QString> localactivityversions;
    QJsonObject jsonroot;
    QJsonObject remoteroot;
private:

    static DataBase* _instance;
    void saveinfo();
};

#endif // DATABASE_H
