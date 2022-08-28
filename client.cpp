#include "client.h"

#include <QNetworkInterface>
#include <QTextStream>


Client::Client(QObject *parent)
    : QObject{parent}
    , peerConn(nullptr)
{
    isResolutionSent = false;
    connect(&server, &Server::newConnection, this, &Client::connectToPeerConn);
    connect(&framePacker, &FramePacker::framePacked, &frameUnpacker, &FrameUnpacker::unpackFrame);
    connect(&frameUnpacker, &FrameUnpacker::frameUnpacked, this, &Client::recvFrame);
}

Client::Client::~Client()
{
    if (peerConn != nullptr)
        delete peerConn;
}

QString Client::getListenIpPort()
{
    QString ipAndPort;
    QTextStream stream = QTextStream(&ipAndPort);
    QString port = QString::number(server.serverPort());
    bool isFirst = true;
    const QList<QHostAddress> &allAddresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : allAddresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback())
        {
            if (!isFirst)
                stream << "\t";
            stream << address.toString() << ":" << port;
            isFirst = false;
        }
    }
    return stream.readAll();
}

void Client::connectToPeerConn(Connection *connection)
{
    qDebug() << "incoming conn";
    if (peerConn != nullptr)
        delete peerConn;
    peerConn = connection;
    connect(peerConn, &Connection::recvWidth, this, &Client::recvWidth);
    connect(peerConn, &Connection::recvHeight, this, &Client::recvHeight);
    connect(peerConn, &Connection::readyForUse, this, &Client::readyForUse);
//    peerConn->sendMessage();
}

void Client::connectToPeerAddrPort(QHostAddress addr, quint16 port)
{
    qDebug() << "outcoming conn";
    if (peerConn == nullptr)
        peerConn = new Connection();

    peerConn->connectToHost(addr, port);
    connect(peerConn, &Connection::recvWidth, this, &Client::recvWidth);
    connect(peerConn, &Connection::recvHeight, this, &Client::recvHeight);
    connect(peerConn, &Connection::readyForUse, this, &Client::readyForUse);

}

void Client::readyForUse()
{
    connect(peerConn,  &Connection::newMessage,
            this, &Client::newMessage);
    QString str = "hey!";
    peerConn->sendMessage(str);
    str = "hi!";
    peerConn->sendMessage(str);
    str = "yay!";
    peerConn->sendMessage(str);
}

void Client::recvWidth(int width)
{
    qDebug() << "recv w = " << width;
}

void Client::recvHeight(int height)
{
    qDebug() << "recv h = " << height;
}

void Client::sendFrame(QVideoFrame frame)
{
    if (peerConn == nullptr)
        return;
    framePacker.packFrame(frame);
    if (!isResolutionSent)
    {
        peerConn->sendResolution(framePacker.getFrameWidth(), framePacker.getFrameHeight());
        isResolutionSent = true;
    }
}
