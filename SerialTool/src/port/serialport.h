#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "abstractport.h"

class QSerialPort;
class QSettings;

namespace Ui {
class SerialPort;
}

class SerialPort : public AbstractPort
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = nullptr);
    ~SerialPort();
    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void setVisibleWidget(bool status);
    bool open();
    void close();
    QByteArray readAll();
    void write(const QByteArray &data);
    bool portStatus(QString *string);
    bool isOpen();
    bool usePortSetDialog() { return true; }
    void portSetDialog();
    QString autoOpen(bool open);

private:
    void scanPort();

private slots:
    void onTimerUpdate();
    void setBaudRate(const QString &string);

#if defined(Q_OS_LINUX)
    void onPortTextEdited();
#endif

private:
    Ui::SerialPort *ui;
    QSerialPort *serialPort;
    QTimer *m_scanTimer;
    QString autoOpenPortName;
};

class SerialPortFactory : public AbstractPortFactory
{
public:
    AbstractPort* create(QWidget *parent = nullptr) { return new SerialPort(parent); }
    QString type() { return "Serial Port"; }
};


#endif // SERIALPORT_H
