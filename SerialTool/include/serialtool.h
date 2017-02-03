#ifndef __SERIALTOOL_H
#define __SERIALTOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_serialtool.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include "channelitem.h"

#define CH_NUM 16

class SerialTool : public QMainWindow
{
    Q_OBJECT

public:
    SerialTool(QWidget *parent = Q_NULLPTR);
    ~SerialTool();

    void loadConfig();
    void saveConfig();
    QSettings *getConfig() { return config; }
    void setLanguage(const QString &string);
    void setStyleSheet(const QString &string);

private slots:
    void addData(int channel, double ms, double value);
    void setChannelVisible(int ch, bool on);
    void horzScrollBarMoved(int value);
    void horzScrollBarTriggered();
    void plotMouseMove();
    void setYRateLower(double d);
    void setYRateUpper(double d);
    void setXRange(const QString &str);
    void channelStyleChanged(ChannelItem *item);
    void tabIndexChanged(int index);
    void tabActionGroupTriggered(QAction *action);
    void realtimeDataSlot();
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
    void onResendBoxChanged(int status);
    void resendTimeChange(int msc);
    void cleanData();
    void setOptions();
    void saveFile();
    void listViewInit();
    void about();
    void onComboBoxChanged(const QString &string);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui_SerialTool ui;
    QString docPath;
    QTimer dataTimer;
    QTimer resendTimer; // 重发时间
    QTimer secTimer;   // 秒定时器
    QSerialPort *serialPort;
    QSettings *config;
    double count[CH_NUM], xRange;
    bool runFlag = true;
    bool replotFlag = true;
    QActionGroup *tabActionGroup;
    int rxCount, txCount;
    QLabel *rxCntLabel, *txCntLabel, *portInfoLabel;
    QByteArray asciiBuf;
    QTranslator appTranslator, qtTranslator;
};

#endif // SERIALTOOL_H
