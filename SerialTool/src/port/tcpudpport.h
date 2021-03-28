#ifndef TCPUDPPORT_H
#define TCPUDPPORT_H

#include "abstractport.h"
#include <QAbstractSocket>

namespace Ui {
class TcpUdpPort;
}

class QSettings;
class QTcpSocket;
class QTcpServer;
class QUdpSocket;
class QHostAddress;

class TcpUdpPort : public AbstractPort
{
    Q_OBJECT

    enum Protocol {
        TCPClient = 0,
        TCPServer,
        UDP
    };

public:
    explicit TcpUdpPort(QWidget *parent = nullptr);
    ~TcpUdpPort();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void setVisibleWidget(bool status);
    bool open();
    void close();
    QByteArray readAll();
    void write(const QByteArray &array);
    bool isOpen(void);
    void retranslate();
    bool portStatus(QString *string);

    QString autoOpen(bool open);
private:
    QString localHost();
    bool openTcpClient();
    bool openTcpServer();
    bool openUdpScoket();
    void readUdpDatagrams();
    QHostAddress hostAddress();

private slots:
    void newConnectSlot();
    void readMessage();
    void onProtocolChanged();
    void ipAddressEdited();
    void removeUserFormList();
    void onError();

private:
    Ui::TcpUdpPort *ui;
    QTcpSocket *tcpClient = nullptr;
    QTcpServer *tcpServer = nullptr;
    QUdpSocket *udpSocket = nullptr;
    QList<QTcpSocket *> listClient;
    Protocol protocol;
    QString serverIP;
    QByteArray readArray;
        QString autoOpenPortName;
};

class TcpUdpPortFactory : public AbstractPortFactory
{
public:
    AbstractPort* create(QWidget *parent = nullptr) { return new TcpUdpPort(parent); }
    QString type() { return "TCP/UDP"; }
};

#endif // TCPUDPPORT_H
