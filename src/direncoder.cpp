#include "direncoder.h"

Direncoder::Direncoder(QString path,QObject *parent)
    : QObject{parent},tpath(path)
{}

void Direncoder::startencodedir()
{
    encodedir(tpath);
}

void Direncoder::encodedir(QString path)
{
    QDir dir(path);
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    QFileInfoList subdirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    static QStringList passstffixs{"txt","gitignore","data","0","1","2","3","4","5","6","7","8","9"};
    for(QFileInfo file:files) {
        QString filepath = file.absoluteFilePath();
        QString suffix = memorybase::getfilesuffix(filepath);
        if(passstffixs.contains(suffix)) continue;
        QFile listfile(path + "/filelist.txt");
        listfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream fout(&listfile);
        fout << "";
        listfile.close();
        EncodeThread *encoder = new EncodeThread(filepath,path + "/filelist.txt");
        threadpool->start(encoder);
    }
    for(QFileInfo subdir:subdirs) {
        if(subdir.baseName() != ".git")
        encodedir(subdir.absoluteFilePath());
    }
    return;
}
