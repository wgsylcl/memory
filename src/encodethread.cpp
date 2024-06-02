#include "encodethread.h"

EncodeThread::EncodeThread(QString filepath,QString filelistpath,QObject *parent)
    : QObject{parent},filelistpath(filelistpath),encryption(QAESEncryption::AES_128,QAESEncryption::ECB,QAESEncryption::PKCS7),filepath(filepath)
{}

void EncodeThread::run()
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QFileInfo fileinfo(file);
    QString filename = fileinfo.fileName();
    TasklogManager::Instance() -> addlog(tr("开始加密 %1 ……").arg(filename));
    QByteArray data = file.readAll();
    file.close();
    QByteArray encodedata = encryption.encode(data,PASSWORD);
    if(encodedata.size() <= chunkSize)
        this -> writesinglefile(encodedata);
    else
        this -> writemultifile(encodedata);
    TasklogManager::Instance() -> addlog(tr("%1 加密完成！").arg(filename));
}

void EncodeThread::writesinglefile(QByteArray &encodedata)
{
    QFile outf(filepath + ".data");
    outf.open(QIODevice::WriteOnly);
    outf.write(encodedata);
    outf.close();
    appendfilelist(filepath + ".data");
}

void EncodeThread::writemultifile(QByteArray &encodedata)
{
    int index = 0;
    for(qint64 i=0 ; i < encodedata.size() ; i += chunkSize) {
        qint64 size = qMin(chunkSize, encodedata.size() - i);
        QByteArray chunk = encodedata.mid(i,size);
        QFile outf(filepath + ".data." + QString::fromStdString(std::to_string(++index)));
        outf.open(QIODevice::WriteOnly);
        outf.write(chunk);
        outf.close();
    }
    QFile outf(filepath + ".data.0");
    outf.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream qout(&outf);
    qout << index;
    outf.close();
    appendfilelist(filepath + ".data.0");
}

void EncodeThread::appendfilelist(QString filepath)
{
    static QMutex appendfilelistlock;
    QMutexLocker appendfilelistlocker(&appendfilelistlock);
    QFile filelistfile(filelistpath);
    filelistfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream qout(&filelistfile);
    qout << QFileInfo(filepath).fileName() << "\n";
    filelistfile.close();
}
