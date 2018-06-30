#include "serialport.h"
#include "ui_serialport.h"
#include "portsetbox.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QDebug>

SerialPort::SerialPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);
    serialPort = new QSerialPort;
    timer = new QTimer;

    QRegExpValidator *pReg = new QRegExpValidator(QRegExp("^\\d{2,7}$"));
    ui->baudRateBox->lineEdit()->setValidator(pReg);

#if defined(Q_OS_LINUX)
    ui->portNameBox->setEditable(true);
#endif

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPort::readyRead);
    connect(timer, &QTimer::timeout, this, &SerialPort::onTimerUpdate);
    connect(ui->baudRateBox, &QComboBox::currentTextChanged, this, &SerialPort::setBaudRate);
    connect(ui->portNameBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(portChanged()));
#if defined(Q_OS_LINUX)
    connect(ui->portNameBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(onPortTextEdited()));
#endif

    scanPort();
    timer->start(1000);
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
    int baudRate = config->value("BaudRate").toInt();
    QString str = QString::number(baudRate);
    config->endGroup();
    serialPort->setBaudRate(baudRate);
    ui->baudRateBox->setCurrentIndex(ui->baudRateBox->findText(str));
    ui->baudRateBox->setCurrentText(str);
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

#if defined(Q_OS_LINUX)
    if (name.indexOf("/dev/") != 0) {
        name = "/dev/" + name;
    }
#endif
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
    PortSetBox portSet(serialPort, this);
    portSet.exec();
}

// 扫描端口
void SerialPort::scanPort()
{
    bool sync = false;
    QComboBox *box = ui->portNameBox;
    QVector<QSerialPortInfo> vec;
    
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        // 检测端口列表变更
        QString str = info.portName() + " (" + info.description() + ")";
        if (box->findText(str) == -1) {
            sync = true;
        }
        vec.append(info);
    }
    // 需要同步或者ui->portNameBox存在无效端口
    if (sync || box->count() != vec.count()) {
        int len = 0;
        QFontMetrics fm(box->font());
        QString text = box->currentText();
        bool edited = box->findText(text) == -1; // only edit enable (linux)
        box->clear();
        for (int i = 0; i < vec.length(); ++i) {
            QString name = vec[i].portName() + " (" + vec[i].description() + ")";
            box->addItem(name);
            int width = fm.boundingRect(name).width(); // 计算字符串的像素宽度
            if (width > len) { // 统计最长字符串
                len = width;
            }
        }
        // 设置当前选中的端口
        if (!text.isEmpty() && (box->findText(text) != -1 || edited)) {
            box->setCurrentText(text);
        } else {
            box->setCurrentIndex(0);
        }
#if defined(Q_OS_LINUX)
        box->lineEdit()->setCursorPosition(0);
#endif
        // 自动控制下拉列表宽度
        box->view()->setMinimumWidth(len + 16);
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

#if defined(Q_OS_LINUX)
void SerialPort::onPortTextEdited()
{
    QComboBox *box = ui->portNameBox;
    QString text = box->currentText();
    if (box->findText(text) == -1) { // can't find text
        qDebug() << "edit text edited";
    }
    box->lineEdit()->setCursorPosition(0);
}
#endif
