#include "direncodemanager.h"

Direncodemanager::Direncodemanager(QObject *parent)
    : QObject{parent}
{
}

Q_INVOKABLE void Direncodemanager::encodedir(QString path)
{
    encoder = new Direncoder(path);
    worker = new QThread(nullptr);
    encoder->moveToThread(worker);
    QObject::connect(worker, &QThread::started, encoder, &Direncoder::startencodedir);
    QObject::connect(worker, &QThread::finished, encoder, &QObject::deleteLater);
    QObject::connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();
}
