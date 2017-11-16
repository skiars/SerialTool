#include "tcpudpport.h"
#include "ui_tcpudpport.h"
#include <QtNetwork>
#include <QMessageBox>
#include <QUdpSocket>

TcpUdpPort::TcpUdpPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpUdpPort)
{
    ui->setupUi(this);

    QRegExp regExpIP("localhost|((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
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

void TcpUdpPort::loadConfig(QSettings *config)
{
    config->beginGroup("TcpUdpPort");
    serverIP = config->value("ServerAddress").toString();
    ui->ipEdit->setText(serverIP);
    ui->portEdit->setText(config->value("PortNumber").toString());
    ui->protocolBox->setCurrentText(config->value("PortProtocol").toString());
    onProtocolChanged();    // update protocol
    config->endGroup();
}

void TcpUdpPort::saveConfig(QSettings *config)
{
    config->beginGroup("TcpUdpPort");
    config->setValue("ServerAddress", QVariant(serverIP));
    config->setValue("PortNumber", QVariant(ui->portEdit->text()));
    config->setValue("PortProtocol", QVariant(ui->protocolBox->currentText()));
    config->endGroup();
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
    const QString& protocolName = ui->protocolBox->currentText();

    if (protocolName == "TCP Client") {
        protocol = TCPClient;
        ui->ipEdit->setReadOnly(false);
        ui->ipEdit->setText(serverIP);
    } else if (protocolName == "TCP Server") {
        protocol = TCPServer;
        ui->ipEdit->setReadOnly(true);
        ui->ipEdit->setText(localHost());
    } else if (protocolName == "UDP") {
        protocol = UDP;
        ui->ipEdit->setReadOnly(false);
        ui->ipEdit->setText(serverIP);
    }
    emit protocolChanged();
}

// get localhost address
QString TcpUdpPort::localHost()
{
    QString ip;

    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach (QHostAddress address, info.addresses()) {
         if(address.protocol() == QAbstractSocket::IPv4Protocol) {
            ip = address.toString();
         }
    }
    return ip;
}

bool TcpUdpPort::openTcpClient()
{
    if (ui->ipEdit->text().isEmpty() || ui->portEdit->text().isEmpty()) {
        QMessageBox err(QMessageBox::Critical,
            tr("Error"), tr("Please enter a valid IP address and port number.\n"),
            QMessageBox::Cancel, this);
        err.exec();
        return false; // Returns false when the IP address or port number is wrong.
    }
    tcpClient = new QTcpSocket();
    tcpClient->connectToHost(hostAddress(), ui->portEdit->text().toInt());
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
        return false; // Return false when network error.
    }
    ui->ipEdit->setReadOnly(true);
    connect(tcpClient, SIGNAL(readyRead()), this,SLOT(readMessage()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
    return true;
}

bool TcpUdpPort::openTcpServer()
{
    if (ui->portEdit->text().isEmpty()) {
        QMessageBox err(QMessageBox::Critical,
            tr("Error"), tr("Please enter a valid port number!\n"),
            QMessageBox::Cancel, this);
        err.exec();
        return false; // Returns false when the port number is empty.
    }
    tcpServer = new QTcpServer();
    if(!tcpServer->listen(QHostAddress::Any, ui->portEdit->text().toInt())) {
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
           return false; // Return false when network error.
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectSlot()));
    return true;
}

bool TcpUdpPort::openUdpScoket()
{
    if (ui->ipEdit->text().isEmpty() || ui->portEdit->text().isEmpty()) {
        QMessageBox err(QMessageBox::Critical,
            tr("Error"), tr("Please enter a valid IP address and port number.\n"),
            QMessageBox::Cancel, this);
        err.exec();
        return false; // Returns false when the IP address or port number is wrong.
    }
    udpSocket = new QUdpSocket();
    if (udpSocket->bind(ui->portEdit->text().toInt()) == false) {
        QMessageBox err(QMessageBox::Critical,
            tr("Error"),
            tr("The port is occupied, Please re-enter it."),
            QMessageBox::Cancel, this);
        err.exec();
        return false; // Return false when the port is occupied.
    }
    ui->ipEdit->setReadOnly(true);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    return true; // return, because the UDP protocol does not need to connect.
}

bool TcpUdpPort::open()
{
    bool status = false;

    switch (protocol) {
    case TCPClient:
        status = openTcpClient();
        break;
    case TCPServer:
        status = openTcpServer();
        break;
    case UDP:
        status = openUdpScoket();
        break;
    default:
        break;
    }
    if (status) {
        ui->protocolBox->setEnabled(false);
        ui->portEdit->setReadOnly(true);
    }
    return status;
}

void TcpUdpPort::close()
{
    switch (protocol) {
    case TCPClient:
        if (tcpClient) {
            tcpClient->close();
            tcpClient->deleteLater();
            tcpClient = NULL;
        }
        break;
    case TCPServer:
        if (tcpServer) {
            tcpServer->close();
            tcpServer->deleteLater();
            tcpServer = NULL;
        }
        listClient.clear();
        break;
    case UDP:
        if (udpSocket) {
            udpSocket->deleteLater();
            udpSocket = NULL;
        }
        break;
    default:
        break;
    }
    ui->protocolBox->setEnabled(true);
    ui->ipEdit->setReadOnly(false);
    ui->portEdit->setReadOnly(false);
}

void TcpUdpPort::newConnectSlot()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    listClient.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(socket, SIGNAL(disconnected()),  socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(removeUserFormList()));
}

void TcpUdpPort::readUdpDatagrams()
{
    QByteArray datagram;

    do {
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
    } while (udpSocket->hasPendingDatagrams());
    readArray.append(datagram);
}

void TcpUdpPort::readMessage()
{
    QObject *socket = sender();
    switch (protocol) {
    case TCPClient:
        readArray.append(tcpClient->readAll());
        break;
    case TCPServer:
        readArray.append(static_cast<QTcpSocket*>(socket)->readAll());
        break;
    case UDP:
        readUdpDatagrams();
        break;
    default:
        break;
    }
    emit readyRead();
}

void TcpUdpPort::removeUserFormList()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->close();
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
            tcpClient->write(array);
        }
        break;
    case TCPServer:
        if (tcpServer) {
            foreach (QTcpSocket *socket, listClient) {
                socket->write(array);
            }
        }
        break;
    case UDP:
        if (udpSocket) {
            udpSocket->writeDatagram(array, hostAddress(), ui->portEdit->text().toInt());
        }
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
    case UDP:
        return udpSocket != NULL;
    default:
        return false;
    }
}

void TcpUdpPort::retranslate()
{
    ui->retranslateUi(this);
}

bool TcpUdpPort::portStatus(QString &string)
{
    bool status;

    string += ui->protocolBox->currentText() + " ";
    if (this->isOpen()) {
        string += "OPEND @" + ui->ipEdit->text()
               + ":" + ui->portEdit->text();
        status = true;
    } else {
        string += "CLOSED";
        status = false;
    }
    return status;
}

QHostAddress TcpUdpPort::hostAddress() {
    QString address = serverIP == "localhost" ? localHost() : serverIP;
    return QHostAddress(address);
}

void TcpUdpPort::onError()
{
    QMessageBox err(QMessageBox::Critical,
        tr("Error"),
        tr("The remote host closed the connection."),
        QMessageBox::Cancel, this);
    err.exec();
    emit connectionError();
}
