#include "firstframepicker.h"

QImage FirstFramePicker::getFirstFrame()
{
    player.setVideoSink(&videoSink);
    player.setSource(videoPath);
    QObject::connect(&videoSink, &QVideoSink::videoFrameChanged, this, &FirstFramePicker::pickFirstFrame);
    player.play();
    loop.exec();
    return this->firstframe;
}

void FirstFramePicker::pickFirstFrame(QVideoFrame frame)
{
    if (this->firstframe.isNull() && frame.isValid())
    {
        this->firstframe = frame.toImage();
        loop.exit();
        QObject::disconnect(&videoSink, &QVideoSink::videoFrameChanged, this, &FirstFramePicker::pickFirstFrame);
        player.pause();
        player.stop();
    }
}