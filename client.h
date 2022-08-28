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
    void newMessage(const QString &from, const QString &message);

public slots:
    void connectToPeerConn(Connection *connection);
private slots:
    void readyForUse();
private:
    Server server;
    Connection *peerConn;
};

#endif // CLIENT_H
