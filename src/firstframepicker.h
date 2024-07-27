#ifndef FIRSTFRAMEPICKER_H
#define FIRSTFRAMEPICKER_H

#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoFrame>
#include <QEventLoop>

class FirstFramePicker : public QObject {
    Q_OBJECT
public:
    explicit FirstFramePicker(QUrl videoPath,QObject *parent = nullptr) : QObject(parent), videoPath(videoPath) {}
    QImage getFirstFrame();
signals:

public slots:
    void pickFirstFrame(QVideoFrame frame);
private:
    QMediaPlayer player;
    QVideoSink videoSink;
    QImage firstframe;
    QUrl videoPath;
    QEventLoop loop;
};

#endif // FIRSTFRAMEPICKER_H