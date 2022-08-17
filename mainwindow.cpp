#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMediaDevices>
#include <QCameraDevice>
#include <QMediaFormat>
#include <QVideoWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCameraDevice camDev = QMediaDevices::defaultVideoInput(); // choose the default camera
    if (camDev.isNull())
    {
        qDebug() << "err: Camera device not found.";
    }
    cam = new QCamera(camDev);
    captureSession.setCamera(cam);
    mediaRecorder = new QMediaRecorder;
    mediaRecorder->setVideoResolution(640,480);
    mediaRecorder->setQuality(QMediaRecorder::Quality::HighQuality);
    mediaRecorder->setVideoFrameRate(24.0);
    captureSession.setRecorder(mediaRecorder);

    connect(ui->StartButton, &QPushButton::clicked, this, &MainWindow::startButtonPressed);
    connect(ui->StopButton, &QPushButton::clicked, this, &MainWindow::stopButtonPressed);

    cam->start();
    //captureSession.setVideoOutput(ui->viewfinder); // for some reason returns Format_Invalid when video sink is specified
    videoSink = new QVideoSink();
    captureSession.setVideoSink(videoSink);
    frameUnpacker.setImgWidth(framePacker.getFrameWidth());
    frameUnpacker.setImgHeight(framePacker.getFrameHeight());
    connect(videoSink, &QVideoSink::videoFrameChanged, &framePacker, &FramePacker::packFrame);
    connect(&framePacker, &FramePacker::framePacked, &frameUnpacker, &FrameUnpacker::unpackFrame); // crashes process...
    connect(&frameUnpacker, &FrameUnpacker::frameUnpacked, this, &MainWindow::showVideoFrame);

    ui->viewfinder->show();
}

MainWindow::~MainWindow()
{
    delete videoSink;
    delete mediaRecorder;
    delete cam;
    delete ui;
}

void MainWindow::startButtonPressed()
{
    ui->StartButton->setEnabled(false);
    ui->StopButton->setEnabled(true);

    mediaRecorder->record();
}

void MainWindow::stopButtonPressed()
{
    ui->StopButton->setEnabled(false);
    ui->StartButton->setEnabled(true);

    mediaRecorder->stop();
}

void MainWindow::showVideoFrame(QImage img)
{
    ui->video->setPixmap(QPixmap::fromImage(img));
}
