#include "connection.h"


/*
 * Protocol is defined as follows, using the CBOR Data Definition Language:
 *
 *  protocol    = [
 *     command
 *  ]
 *  command     = descr / data
 *  undefined   = { 0 => null }
 *  plaintext   = { 1 => text }
 *  width       = { 2 => (int)width }
 *  height      = { 3 => (int)height }
 *  data        = { 4 => bytearray }
 */

Connection::Connection(QObject *parent)
    : QTcpSocket(parent), writer(this)
{
    currentDataType = Undefined;

    connect(this, &QTcpSocket::readyRead, this,
            &Connection::processReadyRead);
//    connect(this, &QTcpSocket::disconnected,
//            &pingTimer, &QTimer::stop);
}

Connection::Connection(qintptr socketDescriptor, QObject *parent)
    : Connection(parent)
{
    setSocketDescriptor(socketDescriptor);
    reader.setDevice(this);
}

bool Connection::sendResolution(int width, int height)
{
    writer.startMap(2);
    writer.append(Width);
    writer.append(width);
    writer.append(Height);
    writer.append(height);
    writer.endMap();
    return true;
}

bool Connection::sendFrame(const QByteArray &data)
{
    if (data.isEmpty())
        return false;

    writer.startMap(1);
    writer.append(Data);
    writer.append(data);
    writer.endMap();
    return true;
}

bool Connection::sendMessage() // for debug purposes
{
    writer.startMap(1);
    writer.append(PlainText);
    writer.append("hey!");
    writer.endMap();
    return true;
}

void Connection::processReadyRead()
{
    // we've got more data, let's parse
    reader.reparse();
    while (reader.lastError() == QCborError::NoError) {
        if (reader.containerDepth() == 0) { // then enter new container
            if (!reader.isMap() || !reader.isLengthKnown())
                break;                  // protocol error
            reader.enterContainer();
        } else if (currentDataType == Undefined) {
            // Current state: read command ID
            // Next state: read command payload
            if (!reader.isInteger())
                break;                  // protocol error
            currentDataType = DataType(reader.toInteger());
            reader.next();
        } else {
            switch (currentDataType) {
            case PlainText:
            {
                if (!reader.isString())
                    return;
                 QCborStreamReader::StringResult<QString> text = reader.readString();
                    qDebug() << "recv = " << text.data;
                break;
            }
            case Width:
            {
                if (!reader.isInteger())
                {
                    qDebug() << "error";
                    reader.clear();
                    return;
                }
                int width = reader.toInteger();
                reader.next();
                if (!reader.isInteger())
                    return;                  // protocol error
                currentDataType = DataType(reader.toInteger());
                if (currentDataType != Height)
                    return;
                reader.next();
                if (!reader.isInteger())
                {
                    qDebug() << "error";
                    reader.clear();
                    return;
                }
                int height = reader.toInteger();
                reader.next();
                emit setImgResolution(width, height);
                break;
            }
            case Data:
            {
                QCborStreamReader::StringResult<QByteArray> imgData = reader.readByteArray();
                if (imgData.status == QCborStreamReader::Ok)
                    emit setImg(imgData.data);
                break;
            }
            default:
                break;
            }
            currentDataType = Undefined;

            if (!reader.hasNext())
                reader.leaveContainer();
        }
    }

    if (reader.lastError() != QCborError::EndOfFile)
        abort();       // parse error
}
