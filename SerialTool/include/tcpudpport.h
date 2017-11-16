#ifndef TCPUDPPORT_H
#define TCPUDPPORT_H

#include <QWidget>
#include <QAbstractSocket>

namespace Ui {
class TcpUdpPort;
}

class QSettings;
class QTcpSocket;
class QTcpServer;
class QUdpSocket;
class QHostAddress;

class TcpUdpPort : public QWidget
{
    Q_OBJECT

    enum Protocol {
        TCPClient = 0,
        TCPServer,
        UDP
    };

public:
    explicit TcpUdpPort(QWidget *parent = 0);
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
    bool portStatus(QString &string);

signals:
    void protocolChanged();
    void readyRead();
    void connectionError();

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
    QTcpSocket *tcpClient = NULL;
    QTcpServer *tcpServer = NULL;
    QUdpSocket *udpSocket = NULL;
    QList<QTcpSocket *> listClient;
    Protocol protocol;
    QString serverIP;
    QByteArray readArray;
};

#endif // TCPUDPPORT_H
