#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include "connection.h"
#include "framepacker.h"

#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QDebug>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    QString getListenIpPort();
    void connectToPeerAddrPort(QHostAddress, quint16);
signals:
    void newMessage(const QString &from, const QString &message);
    void recvFrame(QImage);

public slots:
    void connectToPeerConn(Connection *connection);
    void sendFrame(QVideoFrame);
private slots:
    void readyForUse();
    void recvWidth(int);
    void recvHeight(int);
private:
    Server server;
    Connection *peerConn;
    FramePacker framePacker;
    FrameUnpacker frameUnpacker;
    bool isResolutionSent;
};

#endif // CLIENT_H
