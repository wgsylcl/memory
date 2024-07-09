#include "uploader.h"

Uploader::Uploader(QObject *parent)
    : QObject{parent}, taskfile(runtimedir + "/upload/task.json"), uploaddir(runtimedir + "/upload"), packing(false), packer(nullptr)
{
    QDir dir(uploaddir);
    if (!dir.exists())
        dir.mkpath(uploaddir);
    readtaskfile();
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, this, &Uploader::savefile);
}

Uploader::~Uploader()
{
    if (packing)
        packer->exit(), packer->deleteLater();
}

Q_INVOKABLE void Uploader::addreviewtostudent(QString sender, QString sendto, QString text)
{
    UploadTask task;
    sender = sender.trimmed();
    lastsender = sender;
    if (sender.isEmpty())
        sender = "匿名";
    task.sender = sender;
    task.sendto = sendto;
    task.text = text;
    task.type = UploadTaskType::reviewtostudent;
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE void Uploader::addreviewtoteacher(QString sender, QString sendto, QString text)
{
    UploadTask task;
    sender = sender.trimmed();
    lastsender = sender;
    if (sender.isEmpty())
        sender = "匿名";
    task.sender = sender;
    task.sendto = sendto;
    task.text = text;
    task.type = UploadTaskType::reviewtoteacher;
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE void Uploader::addstudentpicture(QString name, QStringList paths)
{
    UploadTask task;
    task.sender = name;
    task.sendto = name;
    task.type = UploadTaskType::picturetostudent;
    for (QString path : paths)
    {
        QString newfilename = memorybase::generaterandomqstring() + "." + memorybase::getfilesuffix(path);
        memorybase::copyfile(path, runtimedir + "/upload/" + newfilename);
        task.paths.append(newfilename);
    }
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE void Uploader::addteacherpicture(QString name, QStringList paths)
{
    UploadTask task;
    task.sender = name;
    task.sendto = name;
    task.type = UploadTaskType::picturetoteacher;
    for (QString path : paths)
    {
        QString newfilename = memorybase::generaterandomqstring() + "." + memorybase::getfilesuffix(path);
        memorybase::copyfile(path, runtimedir + "/upload/" + newfilename);
        task.paths.append(newfilename);
    }
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE void Uploader::addstudentbirthday(QString name, QString date)
{
    UploadTask task;
    task.sender = name;
    task.sendto = name;
    task.text = date;
    task.type = UploadTaskType::birthdaytostudent;
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE void Uploader::addteacherpetphrase(QString name, QString text)
{
    UploadTask task;
    task.sender = name;
    task.sendto = name;
    task.text = text;
    task.type = UploadTaskType::petphrasetoteacher;
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE void Uploader::addactivitypicture(QString name, QStringList paths)
{
    UploadTask task;
    task.sender = name;
    task.sendto = name;
    task.type = UploadTaskType::picturetoactivity;
    for (QString path : paths)
    {
        QString newfilename = memorybase::generaterandomqstring() + "." + memorybase::getfilesuffix(path);
        memorybase::copyfile(path, runtimedir + "/upload/" + newfilename);
        task.paths.append(newfilename);
    }
    tasks.insert(memorybase::generaterandomqstring(), task);
}

void Uploader::savefile()
{
    QMutexLocker locker(&lock);
    QJsonArray jtasksarray;
    for (UploadTask task : tasks)
    {
        QJsonObject jtask;
        jtask["tasktype"] = task.type;
        jtask["sender"] = task.sender;
        jtask["sendto"] = task.sendto;
        jtask["text"] = task.text;
        QJsonArray jpaths;
        for (QString path : task.paths)
            jpaths.append(path);
        jtask["paths"] = jpaths;
        jtasksarray.append(jtask);
    }
    QJsonObject root;
    root["tasks"] = jtasksarray;
    QJsonDocument jdoc;
    jdoc.setObject(root);
    taskfile.open(QIODevice::WriteOnly);
    taskfile.write(jdoc.toJson());
    taskfile.close();
}

void Uploader::readtaskfile()
{
    QMutexLocker locker(&lock);
    if (!taskfile.exists())
        return;
    taskfile.open(QIODevice::ReadOnly);
    QJsonArray jtaskarray = QJsonDocument::fromJson(taskfile.readAll()).object().value("tasks").toArray();
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
        tasks.insert(memorybase::generaterandomqstring(), task);
    }
    taskfile.close();
}

Q_INVOKABLE void Uploader::addstudentprofile(QString name, QString text)
{
    UploadTask task;
    task.sender = name;
    task.sendto = name;
    task.text = text;
    task.type = UploadTaskType::profiletostudent;
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE void Uploader::addtimeline(QString time, QString text)
{
    UploadTask task;
    task.sender = time;
    task.sendto = time;
    task.text = text;
    task.type = UploadTaskType::timeline;
    tasks.insert(memorybase::generaterandomqstring(), task);
}

Q_INVOKABLE int Uploader::gettaskcount()
{
    return tasks.size();
}

Q_INVOKABLE QStringList Uploader::getalltaskids()
{
    QStringList ret;
    for (auto it = tasks.begin(); it != tasks.end(); it++)
        ret.append(it.key());
    return ret;
}

Q_INVOKABLE int Uploader::gettasktype(QString id)
{
    return tasks[id].type;
}

Q_INVOKABLE QString Uploader::getsender(QString id)
{
    return tasks[id].sender;
}

Q_INVOKABLE QString Uploader::getsendto(QString id)
{
    return tasks[id].sendto;
}

Q_INVOKABLE QString Uploader::gettext(QString id)
{
    return tasks[id].text;
}

Q_INVOKABLE QStringList Uploader::geturls(QString id)
{
    QStringList urls;
    for (QString path : tasks[id].paths)
        urls.append("image://uploadpreviewimageprovider/upload/" + path);
    return urls;
}

Q_INVOKABLE QUrl Uploader::tourl(QString str)
{
    return QUrl(str);
}

Q_INVOKABLE void Uploader::packup()
{
    savefile();
    packer = new Packer(runtimedir + "/upload", memorybase::getsystemdownloadpath() + "/upload_" + memorybase::generaterandomqstring() + ".muf");
    QObject::connect(packer, &Packer::packupfinished, [this](QUrl packfileurl)
                     {
        this->packing = false;
        emit packupfinished(memorybase::toUrl(memorybase::getsystemdownloadpath()));
        QFileInfoList fileinfos = memorybase::getfileinfolist(runtimedir + "/upload");
        for(QFileInfo fileinfo:fileinfos)
        {
            QFile file(fileinfo.absoluteFilePath());
            file.remove();
        }
        this->tasks.clear(); });
    packing = true;
    packer->start();
}

Q_INVOKABLE void Uploader::removetask(QString id)
{
    for (QString path : tasks[id].paths)
        QFile(runtimedir + "/upload/" + path).remove();
    tasks.remove(id);
}

Q_INVOKABLE bool Uploader::is_packing()
{
    return packing;
}

Q_INVOKABLE QString Uploader::getlastsender()
{
    return lastsender;
}

Q_INVOKABLE void Uploader::removepicture(QString taskid, QUrl url)
{
    QString filename = url.fileName();
    tasks[taskid].paths.removeOne(filename);
    memorybase::removefile(runtimedir + "/upload/" + filename);
}

Q_INVOKABLE QUrl Uploader::getlocalmediaurl(QUrl url)
{
    return memorybase::toUrl(runtimedir + "/upload/" + url.fileName());
}
