#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QWidget>

class QSerialPort;
class QSettings;

namespace Ui {
class SerialPort;
}

class SerialPort : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = 0);
    ~SerialPort();
    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void setVisibleWidget(bool status);
    bool open();
    void close();
    QByteArray readAll();
    void write(const QByteArray &data);
    bool portStatus(QString &string);
    bool isOpen();
    void portSetDialog();

signals:
    void readyRead();
    void portError();
    void portChanged();

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
    QTimer *timer;
};

#endif // SERIALPORT_H
