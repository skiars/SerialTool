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
class SerialPort;

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
    void onSecTimerTimeout();
    void openPort();
    void closePort();
    void onPortSwitchActionTriggered();
    void openSetPortInfoBox();
    void readPortData();
    void writePort(const QByteArray &array);
    void cleanData();
    void setOptions();
    void saveFile();
    void about();
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
    void setTabActionIndex(int index);

private:
    Ui_SerialTool ui;
    QString docPath;
    QTimer secTimer;   // 秒定时器
    TcpUdpPort *tcpUdpPort; // TCP/UDP端口
    QSettings *config;
    bool runFlag = true;
    QActionGroup *tabActionGroup;
    int rxCount, txCount;
    QLabel *rxCntLabel, *txCntLabel, *portInfoLabel;
    QVector<QTranslator*> translator;
    VedioBox *vedioBox = NULL;
    PortType portType;
    WaveDecode* waveDecode;
    SerialPort *serialPort;
};

#endif // SERIALTOOL_H
