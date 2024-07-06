#include "imageprovider.h"

ImageProvider::ImageProvider()
    : QQuickImageProvider{QQmlImageProviderBase::Image}, playimg(":/res/play.png")
{
    playimg = playimg.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QUrl url = memorybase::toLocalMediaUrl(id);
    QImage image;
    if (memorybase::isvideo(url))
        loadvideo(url, image);
    else
        loadimage(url, image);
    return image;
}

void ImageProvider::loadimage(QUrl url, QImage &image)
{
    image.load(url.toLocalFile());
    if (url.toString().indexOf(".sign") != -1)
        addwatermark("不用于商业用途", image);
}

void ImageProvider::loadvideo(QUrl url, QImage &image)
{
    AVFormatContext *fmt_ctx_ = nullptr;

    // 打开视频文件
    int errCode = avformat_open_input(&fmt_ctx_, url.toLocalFile().toStdString().c_str(), nullptr, nullptr);
    if (errCode != 0)
    {
        qDebug() << "avformat_open_input fail" << errCode;
        return;
    }

    // 读取音视频流信息
    errCode = avformat_find_stream_info(fmt_ctx_, nullptr);
    if (errCode != 0)
    {
        qDebug() << "avformat_find_stream_info fail" << errCode;
        avformat_close_input(&fmt_ctx_);
        return;
    }

    AVPacket *pkt = av_packet_alloc();
    AVFrame *temp_frame = av_frame_alloc();
    SwsContext *sws_ctx = nullptr;
    int ret = 0;
    QImage preview;
    bool preview_done = false;

    for (int i = 0; i < int(fmt_ctx_->nb_streams) && !preview_done; i++)
    {
        // 只处理视频信息
        if (fmt_ctx_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            // 查找视频解码器
            const AVCodec *codec = avcodec_find_decoder(fmt_ctx_->streams[i]->codecpar->codec_id);
            AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
            // 根据提供的编解码器参数的值填充编解码器上下文
            avcodec_parameters_to_context(codec_ctx, fmt_ctx_->streams[i]->codecpar);
            // 打开解码器
            avcodec_open2(codec_ctx, codec, nullptr);

            // 读取帧数据
            while (av_read_frame(fmt_ctx_, pkt) >= 0)
            {
                av_frame_unref(temp_frame);

                // 对视频帧数据进行解码
                while ((ret = avcodec_receive_frame(codec_ctx, temp_frame)) == AVERROR(EAGAIN))
                {
                    ret = avcodec_send_packet(codec_ctx, pkt);
                    if (ret < 0)
                    {
                        qCritical() << "Failed to send packet to decoder." << ret;
                        break;
                    }
                }

                if (ret < 0 && ret != AVERROR_EOF)
                {
                    qDebug() << "Failed to receive packet from decoder." << ret;
                    continue;
                }

                // 等比例缩放
                int dstH = 240;
                int dstW = qRound(dstH * (float(temp_frame->width) / float(temp_frame->height)));
                // 消除可能的告警
                dstH = (dstH >> 4) << 4;
                dstW = (dstW >> 4) << 4;

                sws_ctx = sws_getContext(
                    temp_frame->width,
                    temp_frame->height,
                    static_cast<AVPixelFormat>(temp_frame->format),
                    dstW,
                    dstH,
                    static_cast<AVPixelFormat>(AV_PIX_FMT_RGBA),
                    SWS_FAST_BILINEAR,
                    nullptr,
                    nullptr,
                    nullptr);
                int linesize[AV_NUM_DATA_POINTERS];
                linesize[0] = dstW * 4;

                // 生成图片
                preview = QImage(dstW, dstH, QImage::Format_RGBA8888);
                uint8_t *data = preview.bits();
                sws_scale(sws_ctx,
                          temp_frame->data,
                          temp_frame->linesize,
                          0,
                          temp_frame->height,
                          &data,
                          linesize);
                sws_freeContext(sws_ctx);

                avcodec_free_context(&codec_ctx);
                preview_done = true;
                break;
            }
        }
    }
    // 释放资源
    av_frame_free(&temp_frame);
    av_packet_free(&pkt);
    avformat_close_input(&fmt_ctx_);
    image = preview;
    QPainter painter(&image);
    painter.drawImage(QPoint((image.width() - playimg.width()) / 2, (image.height() - playimg.height()) / 2), playimg);
}

void ImageProvider::addwatermark(QString text, QImage &image)
{
    QFont font("微软雅黑", 4);        // 你可以设置任何你喜欢的字体和大小
    QColor watermarkColor(Qt::white); // 水印颜色
    watermarkColor.setAlpha(150);     // 设置透明度，范围从0（完全透明）到255（完全不透明）

    QPainter painter(&image);
    painter.setFont(font);
    painter.setPen(QPen(watermarkColor)); // 设置画笔颜色
    // painter.setCompositionMode(QPainter::CompositionMode_DestinationIn); // 设置合成模式为目标与源的交集

    // 计算水印的大小
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(text);

    // 计算水印位置（考虑旋转后的中心点）
    int centerX = (image.width() - textRect.width()) / 2;
    int centerY = (image.height() - textRect.height()) / 2;

    // 保存当前的变换状态
    painter.save();

    // 移动到旋转中心
    painter.translate(centerX, centerY);

    // 旋转画布
    painter.rotate(30);

    // 计算旋转后水印的起始位置（考虑到旋转后的偏移）
    QRectF boundingRect = metrics.boundingRect(QRect(0, 0, 0, 0), Qt::AlignCenter, text);
    painter.translate(-boundingRect.center().x(), -boundingRect.center().y());

    // 绘制水印
    painter.drawText(0, 0, text);

    // 恢复之前的变换状态
    painter.restore();
}

ImageProvider::~ImageProvider()
{
}