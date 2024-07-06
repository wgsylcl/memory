#include "dataupdatehelper.h"

QStringList DataUpdateHelper::virtualactivities;

DataUpdateHelper::DataUpdateHelper(QObject *parent)
    : QObject{parent}
{
    if (virtualactivities.isEmpty())
        virtualactivities = database->getAllRemoteActivitieNames();
}

Q_INVOKABLE QString DataUpdateHelper::getrepopath(QString reponame)
{
    return setting.value("repopaths/" + reponame, runtimedir).toString();
}

Q_INVOKABLE void DataUpdateHelper::setrepopath(QString reponame, QString repopath)
{
    setting.setValue("repopaths/" + reponame, repopath);
    if (!QDir(repopath).exists())
        QDir().mkpath(repopath);
}

Q_INVOKABLE QStringList DataUpdateHelper::readuploadfile(QString filepath)
{
    QStringList ret;
    uploadfilebasename = memorybase::getfilebasename(filepath);
    uploaddir = runtimedir + "/cache/" + uploadfilebasename;
    unpackuploadfile(filepath, uploaddir);
    QFile taskfile(uploaddir + "/task.json");
    taskfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonArray jtaskarray = QJsonDocument::fromJson(taskfile.readAll()).object().value("tasks").toArray();
    taskfile.close();
    for (int i = 0; i < jtaskarray.count(); i++)
    {
        QJsonObject jtask = jtaskarray.at(i).toObject();
        UploadTask task;
        task.type = UploadTaskType(jtask.value("tasktype").toInt());
        task.sender = jtask.value("sender").toString();
        task.sendto = jtask.value("sendto").toString();
        task.text = jtask.value("text").toString();
        QJsonArray jpaths = jtask.value("paths").toArray();
        for (int j = 0; j < jpaths.count(); j++)
            task.paths.append(jpaths.at(j).toString());
        QString taskid = memorybase::generaterandomqstring();
        tasks.insert(taskid, task);
        ret.append(taskid);
    }
    return ret;
}

Q_INVOKABLE void DataUpdateHelper::removetask(QString taskid)
{
    for (QString path : tasks[taskid].paths)
        memorybase::removefile(uploaddir + "/" + path);
    tasks.remove(taskid);
}

Q_INVOKABLE QVariantMap DataUpdateHelper::gettask(QString taskid)
{
    UploadTask task = tasks[taskid];
    QVariantMap ret;
    ret.insert("type", int(task.type));
    ret.insert("sender", task.sender);
    ret.insert("sendto", task.sendto);
    ret.insert("text", task.text);
    return ret;
}

Q_INVOKABLE QStringList DataUpdateHelper::getAllRemoteActivityNames()
{
    return virtualactivities;
}

Q_INVOKABLE void DataUpdateHelper::createnewactivity(QString activityname, QString repopath)
{
    if (!virtualactivities.contains(activityname))
        virtualactivities.append(activityname);
    setrepopath(activityname, repopath);
}

Q_INVOKABLE QString DataUpdateHelper::gettaskfilebasename()
{
    return uploadfilebasename;
}

Q_INVOKABLE void DataUpdateHelper::resetactivitytask(QString taskid, QString newactivityname, QString newactivitypath)
{
    tasks[taskid].sender = newactivityname;
    tasks[taskid].sendto = newactivityname;
    createnewactivity(newactivityname, newactivitypath);
}

Q_INVOKABLE void DataUpdateHelper::acceptalltask()
{
    for (UploadTask task : tasks)
    {
        switch (task.type)
        {
        case UploadTaskType::birthdaytostudent:
            acceptstudentbirthday(task.sendto, task.text);
            break;
        case UploadTaskType::petphrasetoteacher:
            acceptteacherpetphrase(task.sendto, task.text);
            break;
        case UploadTaskType::picturetoactivity:
            acceptpictureactivity(task.sendto, task.paths);
            break;
        case UploadTaskType::picturetostudent:
            acceptstudentpicture(task.sendto, task.paths);
            break;
        case UploadTaskType::picturetoteacher:
            acceptteacherpicture(task.sendto, task.paths);
            break;
        case UploadTaskType::profiletostudent:
            acceptstudentprofile(task.sendto, task.text);
            break;
        case UploadTaskType::reviewtostudent:
            acceptstudentreview(task.sender, task.sendto, task.text);
            break;
        case UploadTaskType::reviewtoteacher:
            acceptteacherreview(task.sender, task.sendto, task.text);
            break;
        }
    }
}

Q_INVOKABLE QStringList DataUpdateHelper::getpaths(QString taskid)
{
    QStringList paths;
    for (QString path : tasks[taskid].paths)
        paths.append("image://uploadpreviewimageprovider/cache/" + uploadfilebasename + "/" + path);
    return paths;
}

void DataUpdateHelper::unpackuploadfile(QString infile, QString outfiledir)
{
    QZipReader zipreader(infile);
    QDir().mkpath(outfiledir);
    zipreader.extractAll(outfiledir);
}

void DataUpdateHelper::acceptteacherreview(QString sender, QString sendto, QString text)
{
    QString teacherdatafile = getrepopath("students") + "/teachers.json";
    QJsonDocument jdoc = QJsonDocument::fromJson(readfile(teacherdatafile));
    QJsonObject root = jdoc.object();
    QJsonArray jarr = root.value("datas").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jteacher = jarr.at(i).toObject();
        if (jteacher.value("name").toString() != sendto)
            continue;
        QJsonArray jreviews = jteacher.value("reviews").toArray();
        jreviews.append(QString("%1:%2").arg(sender).arg(text));
        jteacher["reviews"] = jreviews;
        jarr[i] = jteacher;
    }
    root["datas"] = jarr;
    jdoc.setObject(root);
    writefile(teacherdatafile, jdoc.toJson());
}

void DataUpdateHelper::acceptteacherpicture(QString name, QStringList paths)
{
    QString teacherdatafile = getrepopath("students") + "/teachers.json";
    QJsonDocument jdoc = QJsonDocument::fromJson(readfile(teacherdatafile));
    QJsonObject root = jdoc.object();
    QJsonArray jarr = root.value("datas").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jteacher = jarr.at(i).toObject();
        if (jteacher.value("name").toString() != name)
            continue;
        QJsonArray jpictures = jteacher.value("pictures").toArray();
        for (QString path : paths)
        {
            memorybase::copyfile(uploaddir + "/" + path, getrepopath("pictures") + "/" + path);
            jpictures.append(path);
        }
        jteacher["pictures"] = jpictures;
        jarr[i] = jteacher;
    }
    root["datas"] = jarr;
    jdoc.setObject(root);
    writefile(teacherdatafile, jdoc.toJson());
}

void DataUpdateHelper::acceptteacherpetphrase(QString name, QString text)
{
    QString teacherdatafile = getrepopath("students") + "/teachers.json";
    QJsonDocument jdoc = QJsonDocument::fromJson(readfile(teacherdatafile));
    QJsonObject root = jdoc.object();
    QJsonArray jarr = root.value("datas").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jteacher = jarr.at(i).toObject();
        if (jteacher.value("name").toString() != name)
            continue;
        QJsonArray jpetphrases = jteacher.value("petphrase").toArray();
        jpetphrases.append(QString("\"%1\"").arg(text));
        jteacher["petphrase"] = jpetphrases;
        jarr[i] = jteacher;
    }
    root["datas"] = jarr;
    jdoc.setObject(root);
    writefile(teacherdatafile, jdoc.toJson());
}

void DataUpdateHelper::acceptstudentreview(QString sender, QString sendto, QString text)
{
    QString studentdatafile = getrepopath("students") + "/student.json";
    QJsonDocument jdoc = QJsonDocument::fromJson(readfile(studentdatafile));
    QJsonObject root = jdoc.object();
    QJsonArray jarr = root.value("datas").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jstudent = jarr.at(i).toObject();
        if (jstudent.value("name").toString() != sendto)
            continue;
        QJsonArray jreviews = jstudent.value("review").toArray();
        jreviews.append(QString("%1:%2").arg(sender).arg(text));
        jstudent["review"] = jreviews;
        jarr[i] = jstudent;
    }
    root["datas"] = jarr;
    jdoc.setObject(root);
    writefile(studentdatafile, jdoc.toJson());
}

void DataUpdateHelper::acceptstudentpicture(QString name, QStringList paths)
{
    QString studentdatafile = getrepopath("students") + "/student.json";
    QJsonDocument jdoc = QJsonDocument::fromJson(readfile(studentdatafile));
    QJsonObject root = jdoc.object();
    QJsonArray jarr = root.value("datas").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jstudent = jarr.at(i).toObject();
        if (jstudent.value("name").toString() != name)
            continue;
        QJsonArray jpictures = jstudent.value("picture").toArray();
        for (QString path : paths)
        {
            memorybase::copyfile(uploaddir + "/" + path, getrepopath("pictures") + "/" + path);
            jpictures.append(path);
        }
        jstudent["picture"] = jpictures;
        jarr[i] = jstudent;
    }
    root["datas"] = jarr;
    jdoc.setObject(root);
    writefile(studentdatafile, jdoc.toJson());
}

void DataUpdateHelper::acceptstudentprofile(QString name, QString text)
{
    QString studentdatafile = getrepopath("students") + "/student.json";
    QJsonDocument jdoc = QJsonDocument::fromJson(readfile(studentdatafile));
    QJsonObject root = jdoc.object();
    QJsonArray jarr = root.value("datas").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jstudent = jarr.at(i).toObject();
        if (jstudent.value("name").toString() != name)
            continue;
        jstudent["profile"] = text;
        jarr[i] = jstudent;
    }
    root["datas"] = jarr;
    jdoc.setObject(root);
    writefile(studentdatafile, jdoc.toJson());
}

void DataUpdateHelper::acceptstudentbirthday(QString name, QString text)
{
    QString studentdatafile = getrepopath("students") + "/student.json";
    QJsonDocument jdoc = QJsonDocument::fromJson(readfile(studentdatafile));
    QJsonObject root = jdoc.object();
    QJsonArray jarr = root.value("datas").toArray();
    for (int i = 0; i < jarr.count(); i++)
    {
        QJsonObject jstudent = jarr.at(i).toObject();
        if (jstudent.value("name").toString() != name)
            continue;
        jstudent["birthday"] = text;
        jarr[i] = jstudent;
    }
    root["datas"] = jarr;
    jdoc.setObject(root);
    writefile(studentdatafile, jdoc.toJson());
}

void DataUpdateHelper::acceptpictureactivity(QString activityname, QStringList paths)
{
    QString activitydir = getrepopath(activityname);
    for (QString path : paths)
        memorybase::copyfile(uploaddir + "/" + path, activitydir + "/" + path);
}

QByteArray DataUpdateHelper::readfile(QString path)
{
    QFile datafile(path);
    datafile.open(QIODevice::ReadOnly);
    QByteArray data = datafile.readAll();
    datafile.close();
    return data;
}

void DataUpdateHelper::writefile(QString path, QByteArray data)
{
    QFileInfo fileinfo(path);
    QDir dir = fileinfo.absoluteDir();
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());
    QFile datafile(path);
    datafile.open(QIODevice::WriteOnly);
    datafile.write(data);
    datafile.close();
}
