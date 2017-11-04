#ifndef TCPUDPPORT_H
#define TCPUDPPORT_H

#include <QWidget>

namespace Ui {
class TcpUdpPort;
}

class QTcpSocket;
class QTcpServer;

class TcpUdpPort : public QWidget
{
    Q_OBJECT

    enum Protocol {
        TCPClient,
        TCPServer,
        UDP
    };

public:
    explicit TcpUdpPort(QWidget *parent = 0);
    ~TcpUdpPort();

    void setVisibleWidget(bool status);
    bool open();
    void close();
    QByteArray readAll();
    void write(const QByteArray &array);
    bool isOpen(void);
    QString serverAddress();
    int portNumber();
    QString portProtocol();
    void setServerAddress(const QString & string);
    void setPortNumber(int port);
    void setPortProtocol(const QString & string);
    void retranslate();

signals:
    void readyRead();

private slots:
    void newConnectSlot();
    void readMessage();
    void protocolChanged();
    void ipAddressEdited();
    void removeUserFormList();

private:
    Ui::TcpUdpPort *ui;
    QTcpSocket *tcpClient = NULL;
    QTcpServer *tcpServer = NULL;
    QList<QTcpSocket *> listClient;
    Protocol protocol;
    QString serverIP;
    QByteArray readArray;
};

#endif // TCPUDPPORT_H
