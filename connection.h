#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QCborStreamReader>
#include <QCborStreamWriter>
#include <QDebug>


class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    enum DataType {
        Undefined,
        PlainText,
        Width,
        Height,
        Data
    };

    Connection(QObject *parent = nullptr);
    Connection(qintptr socketDescriptor, QObject *parent = nullptr);

    bool sendResolution(int width, int height);
    bool sendFrame(const QByteArray &);
    bool sendMessage();

signals:
    void setImgResolution(int width, int height);
    void setImg(QByteArray img_data);

public slots:
    void processReadyRead();

private:
    QCborStreamReader reader;
    QCborStreamWriter writer;
    DataType currentDataType;
};

#endif // CONNECTION_H
