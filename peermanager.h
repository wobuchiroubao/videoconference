#ifndef PEERMANAGER_H
#define PEERMANAGER_H

#include <QObject>
#include <QHostAddress>
#include <QDebug>


class Client;

class PeerManager : public QObject
{
    Q_OBJECT
public:
    PeerManager(Client *client);

signals:

private:
    Client *client;
    QHostAddress peerAddress;
    QList<QHostAddress> ipAddresses;
    //QUdpSocket broadcastSocket;
    //QTimer broadcastTimer;
    QString username;
    int serverPort;
};

#endif // PEERMANAGER_H
