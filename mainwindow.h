#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
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
    void showImg(int, const QImage&);
    void startButtonPressed();
    void stopButtonPressed();

private:
    Ui::MainWindow *ui;
    QCamera *cam;
    QMediaCaptureSession captureSession;
    QMediaRecorder *mediaRecorder;
};
#endif // MAINWINDOW_H
