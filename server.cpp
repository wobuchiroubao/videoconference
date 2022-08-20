#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::AnyIPv4);
}
