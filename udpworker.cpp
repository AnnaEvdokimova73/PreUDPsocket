#include "udpworker.h"

UDPworker::UDPworker(QObject *parent, int port) : QObject(parent)
{
    bindPort = port;
}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{
    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, bindPort);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}


/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{
    QByteArray data;
    data = datagram.data();

    switch (bindPort)
    {
    case BIND_PORT:
    {
        QDataStream inStr(&data, QIODevice::ReadOnly);
        QDateTime dateTime;
        inStr >> dateTime;

        emit sig_sendTimeToGUI(dateTime);
        break;
    }
    default:
    {
        QString address = datagram.senderAddress().toString();
        int size = data.size();

        emit sig_sendDataToGUI(address, size);
        break;
    }
    }
}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, bindPort);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
        ReadDatagram(datagram);
    }

}
