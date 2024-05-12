#ifndef BASE_H
#define BASE_H

#include <QFileInfo>
#include <QFile>
#include <QImage>
#include <QUrl>
#include <QString>
#include <QPainter>
#include <QCoreApplication>

namespace memorybase {
bool isvideo(QUrl);
QUrl toLocalMediaUrl(QUrl);
QUrl toLocalMediaUrl(QString);
}
#endif // BASE_H