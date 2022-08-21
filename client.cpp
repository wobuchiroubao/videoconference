#include "client.h"

#include <QNetworkInterface>
#include <QTextStream>


Client::Client(QObject *parent)
    : QObject{parent}
    , peerConn(nullptr)
{
    connect(&server, &Server::newConnection, this, &Client::connectToPeerConn);
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
}

void Client::connectToPeerAddrPort(QHostAddress addr, quint16 port)
{
    qDebug() << "outcoming conn";
    if (peerConn == nullptr)
        peerConn = new Connection();

    peerConn->connectToHost(addr, port);
}
