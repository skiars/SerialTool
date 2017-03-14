#ifndef __SERIALTOOL_H
#define __SERIALTOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_serialtool.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include "channelitem.h"

class SerialTool : public QMainWindow
{
    Q_OBJECT

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

private:
    void loadConfig();
    void closeEvent(QCloseEvent *event);

private:
    Ui_SerialTool ui;
    QString docPath;
    QTimer resendTimer; // 重发时间
    QTimer secTimer;   // 秒定时器
    QSerialPort *serialPort;
    QSettings *config;
    bool runFlag = true;
    QActionGroup *tabActionGroup;
    int rxCount, txCount;
    QLabel *rxCntLabel, *txCntLabel, *portInfoLabel;
    QByteArray asciiBuf;
    QTranslator appTranslator, qtTranslator, qsciTranslator;
};

#endif // SERIALTOOL_H
