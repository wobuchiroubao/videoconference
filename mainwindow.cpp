#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMediaDevices>
#include <QCameraDevice>
#include <QMediaFormat>
#include <QVideoWidget>
#include <QLabel>
#include <QString>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->myIp->setText(client.getListenIpPort());

    connect(ui->peerIpInput, &QLineEdit::returnPressed, this, &MainWindow::enterPressed);

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

    //captureSession.setVideoOutput(ui->viewfinder); // for some reason returns Format_Invalid when video sink is specified
    videoSink = new QVideoSink();
    captureSession.setVideoSink(videoSink);
    //frameUnpacker.setImgWidth(framePacker.getFrameWidth());
    //frameUnpacker.setImgHeight(framePacker.getFrameHeight());
//    connect(videoSink, &QVideoSink::videoFrameChanged, &framePacker, &FramePacker::packFrame);
//    connect(&framePacker, &FramePacker::framePacked, &frameUnpacker, &FrameUnpacker::unpackFrame);
//    connect(&frameUnpacker, &FrameUnpacker::frameUnpacked, this, &MainWindow::showVideoFrame);
    connect(videoSink, &QVideoSink::videoFrameChanged, &client, &Client::sendFrame);
    connect(&client, &Client::recvFrame, this, &MainWindow::showVideoFrame);

    connect(&client, &Client::newMessage,
            this, &MainWindow::appendMessage);

    cam->start();
}

MainWindow::~MainWindow()
{
    delete videoSink;
    delete mediaRecorder;
    delete cam;
    delete ui;
}

void MainWindow::enterPressed()
{
    QString text = ui->peerIpInput->text();
    if (text.isEmpty())
        return;

//    if (text.startsWith(QChar('/'))) {
//        QColor color = textEdit->textColor();
//        textEdit->setTextColor(Qt::red);
//        textEdit->append(tr("! Unknown command: %1")
//                         .arg(text.left(text.indexOf(' '))));
//        textEdit->setTextColor(color);
//    } else {
//        client.sendMessage(text);
//        appendMessage(myNickName, text);
//    }

    QStringList textSplit = text.split(':');
    if (textSplit.count() != 2)
        return;
    QHostAddress peerAddr;
    if (!peerAddr.setAddress(textSplit.at(0)))
       return;

    client.connectToPeerAddrPort(peerAddr, textSplit.at(1).toInt());

    ui->peerIpInput->clear();
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

void MainWindow::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    qDebug() << from << ": msg: " << message;
}
