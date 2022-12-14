#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"

#include <QMainWindow>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QVideoSink>
#include<QDebug>
#include <QThread>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //void showImg(int, const QImage&);
public slots:
    void enterPressed();
    void startButtonPressed();
    void stopButtonPressed();
    void showVideoFrame(QImage);
    void appendMessage(const QString &from, const QString &message);
private:
    Ui::MainWindow *ui;
    Client client;
    QCamera *cam;
    QMediaCaptureSession captureSession;
    QMediaRecorder *mediaRecorder;
    QVideoSink *videoSink;
};
#endif // MAINWINDOW_H
