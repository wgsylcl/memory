#include "packer.h"

Packer::Packer(QString packdir, QString savepath, QObject *parent)
    : QThread{parent}, packdir(packdir), savepath(savepath), zipwriter(nullptr)
{
    if (zipwriter)
        delete zipwriter;
}

void Packer::run()
{
    QObject::connect(this,&QThread::finished,this,&QObject::deleteLater);
    zipwriter = new QZipWriter(savepath);
    zipwriter->setCompressionPolicy(QZipWriter::AutoCompress);
    QFileInfoList fileinfos = memorybase::getfileinfolist(packdir);
    for (QFileInfo fileinfo : fileinfos)
    {
        QFile file(fileinfo.absoluteFilePath());
        file.open(QIODevice::ReadOnly);
        zipwriter->addFile(fileinfo.fileName(), file.readAll());
        file.close();
    }
    delete zipwriter;
    zipwriter = nullptr;
    emit packupfinished();
}
