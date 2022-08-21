#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include "connection.h"

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

public slots:
    void connectToPeerConn(Connection *connection);
private:
    Server server;
    Connection *peerConn;
};

#endif // CLIENT_H
