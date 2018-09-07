#include "portsetbox.h"
#include "ui_portsetbox.h"

#define _size(x) (sizeof(x) / sizeof(x[0]))

static const QSerialPort::DataBits DataBits[] = {
    QSerialPort::Data5,
    QSerialPort::Data6,
    QSerialPort::Data7,
    QSerialPort::Data8
};

static const QSerialPort::Parity Parity[] = {
    QSerialPort::NoParity,
    QSerialPort::EvenParity,
    QSerialPort::OddParity,
    QSerialPort::SpaceParity,
    QSerialPort::MarkParity
};

static const QSerialPort::StopBits StopBits[] = {
    QSerialPort::OneStop,
    QSerialPort::OneAndHalfStop,
    QSerialPort::TwoStop
};

static const QSerialPort::FlowControl FlowControl[] = {
    QSerialPort::NoFlowControl,
    QSerialPort::HardwareControl,
    QSerialPort::SoftwareControl
};

PortSetBox::PortSetBox(QSerialPort *port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PortSetBox)
{
    unsigned int i;

    // 不显示问号
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    ui->setupUi(this);
    setFixedSize(200, 200); // 不能伸缩的对话框

    serialPort = port;

    for (i = 0; port->dataBits() != DataBits[i] && i < _size(DataBits); ++i);
    ui->dataBitsBox->setCurrentIndex(i);
    for (i = 0; port->parity() != Parity[i] && i < _size(Parity); ++i);
    ui->paritBox->setCurrentIndex(i);
    for (i = 0; port->stopBits() != StopBits[i] && i < _size(StopBits); ++i);
    ui->stopBitsBox->setCurrentIndex(i);
    for (i = 0; port->flowControl() != FlowControl[i] && i < _size(StopBits); ++i);
    ui->flowControlBox->setCurrentIndex(i);

    connect(ui->dataBitsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataBits(int)));
    connect(ui->paritBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setParity(int)));
    connect(ui->stopBitsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setStopBits(int)));
    connect(ui->flowControlBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setFlowControl(int)));
}

PortSetBox::~PortSetBox()
{
    delete ui;
}

void PortSetBox::setDataBits(int index)
{
    serialPort->setDataBits(DataBits[index]);
}

void PortSetBox::setParity(int index)
{
    serialPort->setParity(Parity[index]);
}

void PortSetBox::setStopBits(int index)
{
    serialPort->setStopBits(StopBits[index]);
}

void PortSetBox::setFlowControl(int index)
{
    serialPort->setFlowControl(FlowControl[index]);
}
