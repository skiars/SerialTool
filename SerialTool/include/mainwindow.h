#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class MainWindow;
}
class QSettings;
class TcpUdpPort;
class WaveDecode;
class SerialPort;
class VedioBox;
class ValueDisplay;
class QTranslator;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum PortType {
        ComPort,
        NetworkPort
    };

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    void loadSettings();
    void saveConfig();
    QSettings *getConfig() { return m_config; }
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
    void openFile();
    void about();
    void openWiki();
    void onVedioBoxTriggered();
    void onVedioBoxDelete();
    void onValueDisplayTriggered();
    void onValueDisplayDelete();
    void onStaysOnTopTriggered();
    void currentTabChanged(int index);
    void dispPortStatus();

private:
    void loadConfig();
    void closeEvent(QCloseEvent *event);
    bool openComPort();
    bool openTcpUdpPort();
    void loadPortTool();
    void setTabActionIndex(int index);
    void setWindowStaysOnTop(bool enabled);

private:
    Ui::MainWindow *ui;
    QString m_docPath;
    QTimer m_timer;   // 秒定时器
    TcpUdpPort *m_tcpUdpPort; // TCP/UDP端口
    QSettings *m_config;
    bool m_runFlag = true;
    QActionGroup *m_tabActionGroup;
    int m_rxCount, m_txCount;
    QLabel *m_rxCntLabel, *m_txCntLabel, *m_portInfoLabel;
    QVector<QTranslator*> m_translator;
    VedioBox *m_vedioBox = NULL;
    ValueDisplay *m_valueDisplay = NULL;
    PortType m_portType;
    SerialPort *m_serialPort;
};

#endif // __MAINWINDOW_H
