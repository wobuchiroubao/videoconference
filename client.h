#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include "peermanager.h"
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
    QString getListenIpPort();
signals:

private:
    PeerManager *peerManager;
    Server server;
    QHostAddress peerAddr;
    Connection *peerConn;
};

#endif // CLIENT_H
