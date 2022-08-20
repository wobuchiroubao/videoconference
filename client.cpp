#include "client.h"

#include <QNetworkInterface>
#include <QTextStream>


Client::Client(QObject *parent)
    : QObject{parent}
{

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
