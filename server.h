#ifndef SERVER_H
#define SERVER_H

#include "connection.h"

#include <QObject>
#include <QTcpServer>
#include <QDebug>


class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

signals:
    void newConnection(Connection *connection);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
