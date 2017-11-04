#include "tcpudpport.h"
#include "ui_tcpudpport.h"
#include <QtNetwork>
#include <QMessageBox>

TcpUdpPort::TcpUdpPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpUdpPort)
{
    ui->setupUi(this);

    QRegExp regExpIP("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    QRegExp regExpNetPort("((6553[0-5])|[655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{3}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])");
    QRegExpValidator *pReg = new QRegExpValidator(regExpIP, this);
    ui->ipEdit->setValidator(pReg);
    pReg = new QRegExpValidator(regExpNetPort, this);
    ui->portEdit->setValidator(pReg);

    protocolChanged();

    connect(ui->protocolBox, &QComboBox::currentTextChanged, this, &TcpUdpPort::onProtocolChanged);
    connect(ui->ipEdit, &QLineEdit::textEdited, this, &TcpUdpPort::ipAddressEdited);
}

TcpUdpPort::~TcpUdpPort()
{
    delete ui;
    delete tcpClient;
    delete tcpServer;
}

void TcpUdpPort::setVisibleWidget(bool status)
{
    ui->label->setVisible(status);
    ui->protocolBox->setVisible(status);
    ui->ipLabel->setVisible(status);
    ui->ipEdit->setVisible(status);
    ui->label_2->setVisible(status);
    ui->portEdit->setVisible(status);
    setVisible(status);
}

void TcpUdpPort::ipAddressEdited()
{
    serverIP = ui->ipEdit->text();
}

void TcpUdpPort::onProtocolChanged()
{
    if (ui->protocolBox->currentText() == "TCP Client") {
        protocol = TCPClient;
        ui->ipEdit->setReadOnly(false);
        ui->ipEdit->setText(serverIP);
    } else if (ui->protocolBox->currentText() == "TCP Server") {
        protocol = TCPServer;
        ui->ipEdit->setReadOnly(true);
        QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
        foreach (QHostAddress address, info.addresses()) {
             if(address.protocol() == QAbstractSocket::IPv4Protocol) {
                ui->ipEdit->setText(address.toString());
             }
        }
    } else if (ui->protocolBox->currentText() == "UDP") {
        ui->ipEdit->setReadOnly(false);
        protocol = UDP;
    }
    emit protocolChanged();
}

bool TcpUdpPort::open()
{
    switch (protocol) {
    case TCPClient:
        if (ui->ipEdit->text().isEmpty() || ui->portEdit->text().isEmpty()) {
            QMessageBox err(QMessageBox::Critical,
                tr("Error"), tr("Please input port number.\n"),
                QMessageBox::Cancel, this);
            err.exec();
            return false;
        }
        tcpClient = new QTcpSocket();
        tcpClient->connectToHost(QHostAddress(ui->ipEdit->text()), ui->portEdit->text().toInt());
        if (!tcpClient->waitForConnected(1000)) {
            tcpClient->close();
            delete tcpClient;
            tcpClient = NULL;
            QMessageBox err(QMessageBox::Critical,
                tr("Error"),
                tr("Can not connect to server!\n"
                    "Please check the network, IP address and port number."),
                QMessageBox::Cancel, this);
            err.exec();
            return false;
        }
        ui->ipEdit->setEnabled(false);
        connect(tcpClient, SIGNAL(readyRead()), this,SLOT(readMessage()));
        break;
    case TCPServer:
        if (ui->portEdit->text().isEmpty()) {
            QMessageBox err(QMessageBox::Critical,
                tr("Error"), tr("Please input IP address and port number!\n"),
                QMessageBox::Cancel, this);
            err.exec();
            return false;
        }
        tcpServer = new QTcpServer();
        if(!tcpServer->listen(QHostAddress::Any, ui->portEdit->text().toInt())) {
               tcpServer->close();
               tcpServer->close();
               delete tcpServer;
               tcpServer = NULL;
               delete tcpServer;
               QMessageBox err(QMessageBox::Critical,
                   tr("Error"),
                   tr("Can not create server!\n"
                      "Please check the port number."),
                   QMessageBox::Cancel, this);
               err.exec();
               return false;
        }
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectSlot()));
        break;
    default:
        return false;
    }
    ui->protocolBox->setEnabled(false);
    ui->portEdit->setEnabled(false);
    return true;
}

void TcpUdpPort::close()
{
    switch (protocol) {
    case TCPClient:
        if (tcpClient) {
            tcpClient->close();
            delete tcpClient;
            tcpClient = NULL;
        }
        break;
    case TCPServer:
        if (tcpServer) {
            tcpServer->close();
            delete tcpServer;
            tcpServer = NULL;
        }
        listClient.clear();
        break;
    default:
        break;
    }
    ui->protocolBox->setEnabled(true);
    ui->ipEdit->setEnabled(true);
    ui->portEdit->setEnabled(true);
}

void TcpUdpPort::newConnectSlot()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    listClient.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(socket, SIGNAL(disconnected()),  socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(removeUserFormList()));
}

void TcpUdpPort::readMessage()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    switch (protocol) {
    case TCPClient:
        readArray.append(tcpClient->readAll());
        break;
    case TCPServer:
        readArray.append(socket->readAll());
        break;
    default:
        break;
    }
    emit readyRead();
}

void TcpUdpPort::removeUserFormList()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    listClient.removeOne(socket);
}

QByteArray TcpUdpPort::readAll()
{
    QByteArray array = readArray;
    readArray.clear();
    return array;
}

void TcpUdpPort::write(const QByteArray &array)
{
    switch (protocol) {
    case TCPClient:
        if (tcpClient) {
            tcpClient->write(array.data(), array.length());
        }
        break;
    case TCPServer:
        if (tcpServer) {
            foreach (QTcpSocket *socket, listClient) {
                socket->write(array.data(), array.length());
            }
        }
        break;
    default:
        break;
    }
}

bool TcpUdpPort::isOpen(void)
{
    switch (protocol) {
    case TCPClient:
        return tcpClient && tcpClient->isOpen();
    case TCPServer:
        return tcpServer && tcpServer->isListening();
    default:
        return false;
    }
}

QString TcpUdpPort::portProtocol()
{
    return ui->protocolBox->currentText();
}

// 服务器IP
QString TcpUdpPort::serverAddress()
{
    return serverIP;
}

// 当前端口的IP
QString TcpUdpPort::portAddress()
{
    return ui->ipEdit->text();
}

int TcpUdpPort::portNumber()
{
    return ui->portEdit->text().toInt();
}

void TcpUdpPort::setServerAddress(const QString & string)
{
    ui->ipEdit->setText(string);
    serverIP =string;
}

void TcpUdpPort::setPortNumber(int port)
{
    ui->portEdit->setText(QString::number(port));
}

void TcpUdpPort::setPortProtocol(const QString & string)
{
    return ui->protocolBox->setCurrentText(string);
}

void TcpUdpPort::retranslate()
{
    ui->retranslateUi(this);
}
