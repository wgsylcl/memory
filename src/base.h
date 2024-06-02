#ifndef BASE_H
#define BASE_H

#include <QFileInfo>
#include <QObject>
#include <QFile>
#include <QImage>
#include <QUrl>
#include <QString>
#include <QPainter>
#include <QDir>
#include <QCoreApplication>
#include <QThreadPool>

#define threadpool QThreadPool::globalInstance()
const qint64 chunkSize = 99 * 1024 * 1024;

namespace memorybase {
bool isvideo(QUrl);
QUrl toLocalMediaUrl(QUrl);
QUrl toLocalMediaUrl(QString);
bool fileexist(QString);
bool direxist(QString);
QUrl toUrl(QString);
QString topath(QUrl);
QString getfilesuffix(QString);
QString getfilesuffix(QUrl);
}
#endif // BASE_H