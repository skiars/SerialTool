#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class MainWindow;
}
class QSettings;
class QTranslator;
class PortManager;
class Controller;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

public slots:
    void loadSettings();

private slots:
    void changeRunFlag();
    void onSecTimerTimeout();
    void openPort();
    void closePort();
    void onPortSwitchActionTriggered();
    void onPortSwitchAction2Triggered();
    void dispPortStatus();
    void clear();
    void removeConfig();
    void setOptions();
    void onStaysOnTopTriggered();
    void about();
    void openWiki();
    void updateSoftware();

private:
    void loadConfig();
    void saveConfig();
    void setLanguage(const QString &string);
    void setStyleSheet(const QString &string);
    void closeEvent(QCloseEvent *event);
    void setWindowStaysOnTop(bool enabled);

private:
    Ui::MainWindow *ui;
    QString m_docPath;
    QTimer m_timer;   // 秒定时器
    QSettings *m_config;
    bool m_runFlag = true;
    QLabel *m_rxCntLabel, *m_txCntLabel, *m_portInfoLabel;
    QVector<QTranslator*> m_translator;
    PortManager *m_port;
    Controller *m_controller;
};

#endif // MAINWINDOW_H
