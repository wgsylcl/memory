#include "imageprovider.h"

ImageProvider::ImageProvider()
    : QQuickImageProvider{QQmlImageProviderBase::Image}, playimg(":/res/play.png")
{}

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
        addwatermark("不可用于商业用途", image);
}

void ImageProvider::loadvideo(QUrl url, QImage &image)
{
    FirstFramePicker picker(url);
    image = picker.getFirstFrame();
    QPainter painter(&image);
    playimg = playimg.scaled(image.width() / 4, image.height() / 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawImage(QPoint((image.width() - playimg.width()) / 2, (image.height() - playimg.height()) / 2), playimg);
}

void ImageProvider::addwatermark(QString text, QImage &image)
{
    QFont font("微软雅黑", 18);       // 你可以设置任何你喜欢的字体和大小
    QColor watermarkColor(Qt::white); // 水印颜色
    watermarkColor.setAlpha(118);     // 设置透明度，范围从0（完全透明）到255（完全不透明）
    font.setBold(true);

    image = image.scaled(880, 880, Qt::KeepAspectRatio, Qt::SmoothTransformation);

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