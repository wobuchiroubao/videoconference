#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>


class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    Connection(qintptr socketDescriptor, QObject *parent = nullptr);

signals:

};

#endif // CONNECTION_H
