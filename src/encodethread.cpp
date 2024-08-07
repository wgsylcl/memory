#include "encodethread.h"

EncodeThread::EncodeThread(QString filepath, QObject *parent)
    : QObject{parent}, filepath(filepath)
{
}

void EncodeThread::run()
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QFileInfo fileinfo(file);
    QString filename = fileinfo.fileName();
    QByteArray encodedata = file.readAll();
    file.close();
    if (encodedata.size() > chunkSize)
        this->writemultifile(encodedata);
}

void EncodeThread::writemultifile(QByteArray &encodedata)
{
    int index = 0;
    for (qint64 i = 0; i < encodedata.size(); i += chunkSize)
    {
        qint64 size = qMin(chunkSize, encodedata.size() - i);
        QByteArray chunk = encodedata.mid(i, size);
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
}
