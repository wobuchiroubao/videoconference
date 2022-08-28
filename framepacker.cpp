#include "framepacker.h"

#include <QImage>


FramePacker::FramePacker(QObject *parent)
    : QObject{parent}
    , frameWidth(0)
    , frameHeight(0)
{}

QByteArray FramePacker::packFrame(QVideoFrame frame)
{
    QImage img = frame.toImage();
    int newWidth = img.width();
    if (frameWidth != newWidth)
    {
        qDebug() << "w = " << newWidth;
        frameWidth = newWidth;
        emit frameWidthChanged(newWidth);
    }
    int newHeight = img.height();
    if (frameHeight != newHeight)
    {
        qDebug() << "h = " << newHeight;
        frameHeight = newHeight;
        emit frameHeightChanged(newHeight);
    }
    format = img.format();
    //qDebug() << "format = " << format;
    return QByteArray((const char*) img.constBits(), img.sizeInBytes());
}

int FramePacker::getFrameWidth() const
{
    return frameWidth;
}

int FramePacker::getFrameHeight() const
{
    return frameHeight;
}

FrameUnpacker::FrameUnpacker(QObject *parent)
    : QObject{parent}
    , imgFormat(QImage::Format::Format_RGB32)
{}

QImage FrameUnpacker::unpackFrame(QByteArray array)
{
    imgWidth = 1280;
    imgHeight = 720;
    //qDebug() << "wid = " << imgWidth;
    //qDebug() << "hei = " << imgHeight;
    //qDebug() << "sum = " << array.count();
    QImage img = QImage(imgWidth, imgHeight, imgFormat);
    memcpy(img.bits(), array.constData(), array.count());
    return img;
}

void FrameUnpacker::setImgWidth(int width)
{
    imgWidth = width;
}

void FrameUnpacker::setImgHeight(int height)
{
    imgHeight = height;
}
