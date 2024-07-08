#include "direncoder.h"

Direncoder::Direncoder(QString path, QObject *parent)
    : QObject{parent}, tpath(path)
{
}

void Direncoder::startencodedir()
{
    encodedir(tpath);
}

void Direncoder::encodedir(QString path)
{
    QDir dir(path);
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    QFileInfoList subdirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    static QStringList passstffixs{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (QFileInfo file : files)
    {
        QString filepath = file.absoluteFilePath();
        QString suffix = memorybase::getfilesuffix(filepath);
        if (passstffixs.contains(suffix))
            continue;
        EncodeThread *encoder = new EncodeThread(filepath);
        threadpool->start(encoder);
    }
    for (QFileInfo subdir : subdirs)
    {
        if (subdir.baseName() != ".git")
            encodedir(subdir.absoluteFilePath());
    }
    return;
}
