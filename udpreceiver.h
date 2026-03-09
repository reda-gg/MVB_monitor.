#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class UdpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UdpReceiver(QObject *parent = nullptr);

signals:
    void frameReceived(QByteArray data);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *socket;
};

#endif
