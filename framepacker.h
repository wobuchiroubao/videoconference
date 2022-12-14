#ifndef FRAMEPACKER_H
#define FRAMEPACKER_H

#include <QObject>
#include <QVideoFrame>
#include <QByteArray>
#include<QDebug>

class FramePacker : public QObject
{
    Q_OBJECT
public:
    explicit FramePacker(QObject *parent = nullptr);
    int getFrameWidth() const;
    int getFrameHeight() const;
    QByteArray packFrame(QVideoFrame);
public slots:

signals:
    //void framePacked(QByteArray, int, int);
    void frameWidthChanged(int newWidth);
    void frameHeightChanged(int newHeight);
private:
    //QVideoFrame bufferedFrame;
    QVideoFrame curFrame;
    int frameWidth;
    int frameHeight;
    QImage::Format format;
};

class FrameUnpacker : public QObject
{
    Q_OBJECT
public:
    explicit FrameUnpacker(QObject *parent = nullptr);
    QImage unpackFrame(QByteArray);
public slots:
    void setImgWidth(int);
    void setImgHeight(int);
signals:
    //void frameUnpacked(QImage);
private:
    int imgWidth;
    int imgHeight;
    QImage::Format imgFormat;
    //QByteArray bufferedArray;
    QByteArray curArray;
};

#endif // FRAMEPACKER_H
