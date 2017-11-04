#ifndef __SERIALTOOL_H
#define __SERIALTOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_serialtool.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include "channelitem.h"
#include "vediobox.h"

class TcpUdpPort;
class WaveDecode;

class SerialTool : public QMainWindow
{
    Q_OBJECT

    enum PortType {
        ComPort,
        NetworkPort
    };

public:
    SerialTool(QWidget *parent = Q_NULLPTR);
    ~SerialTool();

    void loadSettings();
    void saveConfig();
    QSettings *getConfig() { return config; }
    void setLanguage(const QString &string);
    void setStyleSheet(const QString &string);

private slots:
    void tabIndexChanged(int index);
    void tabActionGroupTriggered(QAction *action);
    void changeRunFlag();
    void scanPort();
    void onSecTimerTimeout();
    void openPort();
    void closePort();
    void onPortSwitchActionTriggered();
    void openSetPortInfoBox();
    void setPortBaudRate(const QString &string);
    void onSendButtonClicked();
    void readPortData();
    void writePortData();
    void writePort(const QByteArray &array);
    void onResendBoxChanged(int status);
    void resendTimeChange(int msc);
    void cleanData();
    void setOptions();
    void saveFile();
    void about();
    void onComboBoxChanged(const QString &string);
    void onWrapBoxChanged(int status);
    void onVedioBoxTriggered();
    void onVedioBoxDelete();
    void currentTabChanged(int index);
    void dispPortStatus();

private:
    void loadConfig();
    void closeEvent(QCloseEvent *event);
    bool openComPort();
    bool openTcpUdpPort();
    void loadPortTool();
    void dispComPortStatus(QLabel *label);
    void dispTcpUdpPortStatus(QLabel *label);

private:
    Ui_SerialTool ui;
    QString docPath;
    QTimer resendTimer; // 重发时间
    QTimer secTimer;   // 秒定时器
    TcpUdpPort *tcpUdpPort; // TCP/UDP端口
    QSerialPort *serialPort;
    QSettings *config;
    bool runFlag = true;
    QActionGroup *tabActionGroup;
    int rxCount, txCount;
    QLabel *rxCntLabel, *txCntLabel, *portInfoLabel;
    QByteArray asciiBuf;
    QVector<QTranslator*> translator;
    VedioBox *vedioBox = NULL;
    PortType portType;
    WaveDecode* waveDecode;
};

#endif // SERIALTOOL_H
