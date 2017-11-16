#include "serialport.h"
#include "ui_serialport.h"
#include "portsetbox.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>
#include <QLineEdit>

SerialPort::SerialPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);
    serialPort = new QSerialPort;
    timer = new QTimer;

    scanPort();
    timer->start(1000);

    QRegExpValidator *pReg = new QRegExpValidator(QRegExp("^\\d{2,7}$"));
    ui->baudRateBox->lineEdit()->setValidator(pReg);

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPort::readyRead);
    connect(timer, &QTimer::timeout, this, &SerialPort::onTimerUpdate);
    connect(ui->baudRateBox, &QComboBox::currentTextChanged, this, &SerialPort::setBaudRate);
    connect(ui->portNameBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(portChanged()));
}

SerialPort::~SerialPort()
{
    delete ui;
    delete serialPort;
    delete timer;
}

void SerialPort::retranslate()
{
    ui->retranslateUi(this);
}

void SerialPort::loadConfig(QSettings *config)
{
    config->beginGroup("SerialPort");
    serialPort->setBaudRate(config->value("BaudRate").toInt());
    ui->baudRateBox->setCurrentText(QString::number(serialPort->baudRate()));
    config->endGroup();
}

void SerialPort::saveConfig(QSettings *config)
{
    config->beginGroup("SerialPort");
    config->setValue("BaudRate", QVariant(ui->baudRateBox->currentText()));
    config->endGroup();
}

void SerialPort::setVisibleWidget(bool visible)
{
    ui->labelPort->setVisible(visible);
    ui->labelBaudRate->setVisible(visible);
    ui->portNameBox->setVisible(visible);
    ui->baudRateBox->setVisible(visible);
    this->setVisible(visible);
}

bool SerialPort::open()
{
    QString name = ui->portNameBox->currentText().section(' ', 0, 0);
    serialPort->setPortName(name);
    if (serialPort->open(QIODevice::ReadWrite)) {
        ui->portNameBox->setEnabled(false); // 禁止更改串口
        return true;
    }
    QMessageBox err(QMessageBox::Critical,
        tr("Error"),
        tr("Can not open the port!\n"
            "Port may be occupied or configured incorrectly!"),
        QMessageBox::Cancel, this);
    err.exec();
    return false;
}

void SerialPort::close()
{
    serialPort->close();
    ui->portNameBox->setEnabled(true); // 允许更改串口
}

QByteArray SerialPort::readAll()
{
    return serialPort->readAll();
}

void SerialPort::write(const QByteArray &data)
{
    serialPort->write(data);
}

bool SerialPort::portStatus(QString &string)
{
    bool status;
    static const QString parity[] {
        "None", "Even", "Odd", "Space", "Mark", "Unknown"
    };
    static const QString flowControl[] {
        "None", "RTS/CTS", "XON/XOFF", "Unknown"
    };

    // 获取端口名
    string = ui->portNameBox->currentText().section(" ", 0, 0) + " ";
    if (string == " ") { // 端口名是空的
        string = "COM Port ";
    }
    if (serialPort->isOpen()) {
        string += "OPEND, " + QString::number(serialPort->baudRate()) + "bps, "
               + QString::number(serialPort->dataBits()) + "bit, "
               + parity[serialPort->parity()] + ", "
               + QString::number(serialPort->stopBits()) + ", "
               + flowControl[serialPort->flowControl()];
        status = true;
    } else {
        string += "CLOSED";
        status = false;
    }
    return status;
}

bool SerialPort::isOpen()
{
    return serialPort->isOpen();
}

void SerialPort::portSetDialog()
{
    PortSetBox portSet(serialPort);
    portSet.exec();
}

// 扫描端口
void SerialPort::scanPort()
{
    int i = 0;
    bool sync = false;
    QVector<QSerialPortInfo> vec;
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        // 端口名不在匹配说明端口列表变更
        if (ui->portNameBox->itemText(i++).indexOf(info.portName())) {
            sync = true;
        }
        vec.append(info);
    }
    // 需要同步或者ui->portNameBox存在无效端口
    if (sync || !ui->portNameBox->itemText(i).isEmpty()) {
        int len = 0;
        QFontMetrics fm(ui->portNameBox->font());
        QString str = ui->portNameBox->currentText();
        ui->portNameBox->clear();
        for (i = 0; i < vec.length(); ++i) {
            QString t = vec[i].portName() + " (" + vec[i].description() + ")";
            ui->portNameBox->addItem(t);
            int width = fm.boundingRect(t).width(); // 计算字符串的像素宽度
            if (width > len) { // 统计最长字符串
                len = width;
            }
        }
        if (!str.isEmpty()) { // 设置当前选中的端口
            ui->portNameBox->setCurrentText(str);
        } else {
            ui->portNameBox->setCurrentIndex(0);
        }
        // 自动控制下拉列表宽度
        ui->portNameBox->view()->setMinimumWidth(len + 9);
    }
}

void SerialPort::onTimerUpdate()
{
    if (serialPort->isOpen()) {
        if (serialPort->error() == QSerialPort::NoError) {
            return;
        }
        emit portError();
    }
    scanPort();
}

void SerialPort::setBaudRate(const QString &string)
{
    serialPort->setBaudRate(string.toInt());
}
