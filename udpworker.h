#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define BIND_PORT_DATA 12573

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent, int port);
    void InitSocket();
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );

private slots:
    void readPendingDatagrams(void);

private:
    int bindPort;
    QUdpSocket* serviceUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendDataToGUI(QString address, int size);

};

#endif // UDPWORKER_H
