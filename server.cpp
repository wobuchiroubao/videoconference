#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::AnyIPv4);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "Server::incomingConnection";
    Connection *connection = new Connection(socketDescriptor);
    emit newConnection(connection);
}
