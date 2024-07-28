#ifndef FIRSTFRAMEPICKER_H
#define FIRSTFRAMEPICKER_H

#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoFrame>
#include <QEventLoop>

class FirstFramePicker : public QObject
{
    Q_OBJECT
public:
    explicit FirstFramePicker(QUrl videourl, QObject *parent = nullptr) : QObject(parent), videourl(videourl) {}
    explicit FirstFramePicker(QString videopath, QObject *parent = nullptr) : QObject(parent), videourl(videopath) {}
    QImage getFirstFrame();
signals:

public slots:
    void pickFirstFrame(QVideoFrame frame);

private:
    QMediaPlayer player;
    QVideoSink videoSink;
    QImage firstframe;
    QUrl videourl;
    QEventLoop loop;
};

#endif // FIRSTFRAMEPICKER_H