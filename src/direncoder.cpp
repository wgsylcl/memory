#include "direncoder.h"

Direncoder::Direncoder(QObject *parent)
    : QObject{parent}
{}

Q_INVOKABLE void Direncoder::encodedir(QString path)
{
    QDir dir(path);
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    QFileInfoList subdirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(QFileInfo file:files) {
        EncodeThread *encoder = new EncodeThread(file.absoluteFilePath(),path + "/filelist.txt");
        threadpool->start(encoder);
    }
    for(QFileInfo subdir:subdirs) {
        encodedir(subdir.absoluteFilePath());
    }
    return;
}
