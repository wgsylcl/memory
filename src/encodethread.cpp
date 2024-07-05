#include "encodethread.h"

EncodeThread::EncodeThread(QString filepath,QString filelistpath,QString ignorepath,QMutex &appendfilelistlock,QMutex &appendignorefilelock,QObject *parent)
    : QObject{parent},filelistpath(filelistpath),ignorepath(ignorepath),filepath(filepath),appendfilelistlock(appendfilelistlock),appendignorefilelock(appendignorefilelock)
{}

void EncodeThread::run()
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QFileInfo fileinfo(file);
    QString filename = fileinfo.fileName();
    QByteArray encodedata = file.readAll();
    file.close();
    if(encodedata.size() > chunkSize)
        this -> writemultifile(encodedata);
    else appendfilelist(filename);
}

void EncodeThread::writemultifile(QByteArray &encodedata)
{
    int index = 0;
    for(qint64 i=0 ; i < encodedata.size() ; i += chunkSize) {
        qint64 size = qMin(chunkSize, encodedata.size() - i);
        QByteArray chunk = encodedata.mid(i,size);
        QFile outf(filepath + "." + QString::fromStdString(std::to_string(++index)));
        outf.open(QIODevice::WriteOnly);
        outf.write(chunk);
        outf.close();
    }
    QFile outf(filepath + ".0");
    outf.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream qout(&outf);
    qout << index;
    outf.close();
    appendfilelist(filepath + ".0");
    appendignorefile(filepath);
}

void EncodeThread::appendfilelist(QString filepath)
{
    QMutexLocker appendfilelistlocker(&appendfilelistlock);
    QFile filelistfile(filelistpath);
    filelistfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream qout(&filelistfile);
    qout << QFileInfo(filepath).fileName() << "\n";
    filelistfile.close();
}

void EncodeThread::appendignorefile(QString filepath)
{
    QMutexLocker appendignorefilelocker(&appendignorefilelock);
    QFile filelistfile(ignorepath);
    filelistfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream qout(&filelistfile);
    qout << QFileInfo(filepath).fileName() << "\n";
    filelistfile.close();
}
