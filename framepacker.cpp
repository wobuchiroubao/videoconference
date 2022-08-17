#include "framepacker.h"

#include <QImage>


FramePacker::FramePacker(QObject *parent)
    : QObject{parent}
    , frameWidth(0)
    , frameHeight(0)
{}

void FramePacker::packFrame(QVideoFrame frame)
{
    QImage img = frame.toImage();
    int newWidth = img.width();
    if (frameWidth != newWidth)
    {
        frameWidth = newWidth;
        emit frameWidthChanged(newWidth);
    }
    int newHeight = img.height();
    if (frameHeight != newHeight)
    {
        frameHeight = newHeight;
        emit frameHeightChanged(newHeight);
    }
    format = img.format();
    qDebug() << "format = " << format;
    QByteArray array = QByteArray::fromRawData((const char*) img.constBits(), img.sizeInBytes());
    emit framePacked(array, frameWidth, frameHeight);
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

void FrameUnpacker::unpackFrame(QByteArray array, int width, int height)
{
    imgWidth = width;
    imgHeight = height;
    //qDebug() << "wid = " << imgWidth;
    //qDebug() << "hei = " << imgHeight;
    //qDebug() << "sum = " << array.count();
    QImage img = QImage(imgWidth, imgHeight, imgFormat);
    memcpy(img.bits(), array.constData(), array.count());
    emit frameUnpacked(img);
}

void FrameUnpacker::setImgWidth(int width)
{
    imgWidth = width;
}

void FrameUnpacker::setImgHeight(int height)
{
    imgHeight = height;
}
