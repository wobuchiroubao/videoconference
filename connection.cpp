#include "connection.h"


Connection::Connection(QObject *parent)
    : QTcpSocket(parent)//, writer(this)
{

}

Connection::Connection(qintptr socketDescriptor, QObject *parent)
    : Connection(parent)
{
    setSocketDescriptor(socketDescriptor);
    //reader.setDevice(this);
}
