#include "udpreceiver.h"

UdpReceiver::UdpReceiver(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::AnyIPv4,10550,QUdpSocket::ShareAddress);
    socket->joinMulticastGroup(QHostAddress("239.1.2.3"));

    connect(socket,
            &QUdpSocket::readyRead,
            this,
            &UdpReceiver::processPendingDatagrams);
}

void UdpReceiver::processPendingDatagrams()
{
    while(socket->hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(socket->pendingDatagramSize());

        socket->readDatagram(datagram.data(),datagram.size());

        emit frameReceived(datagram);
    }
}
