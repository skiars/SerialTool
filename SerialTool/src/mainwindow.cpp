#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QTranslator>
#include <QFileDialog>
#include <QDesktopServices>
#include "aboutbox.h"
#include "version.h"
#include "defaultconfig.h"
#include "updatedialog.h"
#include "controller.h"
#include "port/portmanager.h"
#include "settings/optionsdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    QString configPath(QStandardPaths::writableLocation(
//        QStandardPaths::AppConfigLocation) + "/config.ini");

    QString configPath( QCoreApplication::applicationDirPath() + "/config.ini");

    syncDefaultConfig(configPath);
    m_config = new QSettings(configPath, QSettings::IniFormat);

    ui->setupUi(this);
    setWindowTitle(SOFTWARE_TITLE);

    m_port = new PortManager(ui->toolBar1, ui->portSetAction);
    m_controller = new Controller(&m_docPath,
        this, ui->menu_2, ui->actionOpen, ui->actionSave);
    m_controller->setPortManager(m_port);

    // 状态栏设置
    m_rxCntLabel = new QLabel("RX: 0Bytes", this);
    m_txCntLabel = new QLabel("TX: 0Bytes", this);
    m_portInfoLabel = new QLabel("", this);
    m_rxCntLabel->setMinimumWidth(120);
    m_txCntLabel->setMinimumWidth(120);
    m_portInfoLabel->setMinimumWidth(120);
    ui->statusBar->addWidget(m_portInfoLabel);
    ui->statusBar->addWidget(m_rxCntLabel);
    ui->statusBar->addWidget(m_txCntLabel);

    loadConfig(); // 加载配置

    // create connection between axes and scroll bars:
    connect(ui->portRunAction, SIGNAL(triggered()), this, SLOT(changeRunFlag()));
    connect(ui->portSwitchAction, SIGNAL(triggered()), this, SLOT(onPortSwitchActionTriggered()));
    connect(ui->portSwitchAction2, SIGNAL(triggered()), this, SLOT(onPortSwitchAction2Triggered()));
    connect(ui->clearAction, SIGNAL(triggered()), this, SLOT(clear()));
    connect(ui->actionRemoveConfig, SIGNAL(triggered()), this, SLOT(removeConfig()));
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::onSecTimerTimeout);
    connect(ui->actionOption, SIGNAL(triggered()), this, SLOT(setOptions()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionVisibleToolbar, SIGNAL(triggered(bool)), ui->toolBar1, SLOT(setVisible(bool)));
    connect(ui->actionVisibleStatusBar, SIGNAL(triggered(bool)), ui->statusBar, SLOT(setVisible(bool)));
    connect(ui->toolBar1, SIGNAL(visibilityChanged(bool)), ui->actionVisibleToolbar, SLOT(setChecked(bool)));
    connect(m_port, SIGNAL(portChanged()), this, SLOT(dispPortStatus()));
    connect(m_port, SIGNAL(portError()), this, SLOT(closePort()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionWiki, SIGNAL(triggered()), this, SLOT(openWiki()));
    connect(ui->actionStaysOnTop, SIGNAL(triggered()), this, SLOT(onStaysOnTopTriggered()));
    connect(ui->actionUpdate, SIGNAL(triggered()), this, SLOT(updateSoftware()));

    m_timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_config;
    delete m_rxCntLabel;
    delete m_txCntLabel;
    delete m_portInfoLabel;
}

// 关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveConfig();
    QMainWindow::closeEvent(event);
}

// 加载语言
void MainWindow::setLanguage(const QString &string)
{
    // 首先卸载翻译
    if (!m_translator.isEmpty()) {
        for (int i = 0; i < m_translator.size(); ++i) {
            qApp->removeTranslator(m_translator[i]);  // 卸载翻译环境
            delete m_translator[i];
        }
        m_translator.clear();
    }
    // 遍历文件
    QDir dir("language/" + string);
    foreach(QFileInfo mfi ,dir.entryInfoList()) {
        if (mfi.isFile() && mfi.suffix() == "qm") { // 是翻译文件
            QTranslator* ts = new QTranslator;
            ts->load(mfi.absoluteFilePath());
            qApp->installTranslator(ts);    // 安装翻译环境
            m_translator.append(ts);
        }
    }
    // 重新翻译界面
    ui->retranslateUi(this);
    m_port->retranslate();
    m_controller->retranslate();
}

// 加载样式表
void MainWindow::setStyleSheet(const QString &string)
{
    // 样式表
    QFile qss("themes/" + string + "/style.css");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

// 读取系统设置
void MainWindow::loadSettings()
{
    // 系统设置
    m_config->beginGroup("Settings");

    m_port->loadSettings(m_config);
    m_controller->loadSettings(m_config);

    // 窗口透明度
    int opacity = m_config->value("WindowOpacity").toInt();
    opacity = opacity < 30 ? 100 : opacity <= 100 ? opacity : 100;
    setWindowOpacity(opacity / 100.0);
    m_controller->setWindowOpacity(opacity / 100.0);

    // 语言设置
    setLanguage(m_config->value("Language").toString());
    setStyleSheet(m_config->value("Theme").toString());

    m_config->endGroup();

    dispPortStatus(); // 更新端口状态显示
}

// 控件数据初始化, 在构造函数中初始化各种控件的初始值
void MainWindow::loadConfig()
{
    // 路经
    m_config->beginGroup("Path");
    m_docPath = m_config->value("DocumentPath").toString();
    m_config->endGroup();

    m_port->loadConfig(m_config);
    m_controller->loadConfig(m_config);

    // 打开页面配置
    m_config->beginGroup("Workspace");
    restoreGeometry(m_config->value("MainWindowGeometry").toByteArray());
    restoreState(m_config->value("MainWindowState").toByteArray());
    ui->toolBar1->setVisible(m_config->value("ToolBarVisible").toBool());
    ui->actionVisibleToolbar->setChecked(m_config->value("ToolBarVisible").toBool());
    ui->statusBar->setVisible(m_config->value("StatusBarVisible").toBool());
    // 这里如果直接速读取ui->statusBar->isVisible()会是false,原因不明
    ui->actionVisibleStatusBar->setChecked(m_config->value("StatusBarVisible").toBool());
    setWindowStaysOnTop(m_config->value("WindowStaysOnTop").toBool());
    m_config->endGroup();

    // 最后读取系统设置
    loadSettings();
}

// 保存配置
void MainWindow::saveConfig()
{
    // 打开页面配置
    m_config->beginGroup("Workspace");
    m_config->setValue("MainWindowGeometry", saveGeometry());
    m_config->setValue("MainWindowState", saveState());
    m_config->setValue("ToolBarVisible", ui->toolBar1->isVisible());
    m_config->setValue("StatusBarVisible", ui->statusBar->isVisible());
    m_config->setValue("WindowStaysOnTop", (windowFlags() & Qt::WindowStaysOnTopHint) != 0);
    m_config->endGroup();

    // 路经
    m_config->beginGroup("Path");
    m_config->setValue("DocumentPath", QVariant(m_docPath));
    m_config->endGroup();

    m_port->saveConfig(m_config);
    m_controller->saveConfig(m_config);
}

void MainWindow::setOptions()
{
    /*OptionsBox option(this);
    option.exec();*/
    OptionsDialog dialog(m_config, this);
    connect(&dialog, SIGNAL(settingsUpdated()), this, SLOT(loadSettings()));
    dialog.exec();
}

void MainWindow::changeRunFlag()
{
    if (m_runFlag == true) {
        m_runFlag = false;
        QIcon icon(":/SerialTool/images/start.ico");
        ui->portRunAction->setIcon(icon);
        ui->portRunAction->setText(tr("Start Tx/Rx"));
    } else {
        m_runFlag = true;
        QIcon icon(":/SerialTool/images/pause.ico");
        ui->portRunAction->setIcon(icon);
        ui->portRunAction->setText(tr("Pause Tx/Rx"));
    }
    m_controller->setEnabled(m_runFlag);
}

// 状态栏显示端口状态
void MainWindow::dispPortStatus()
{
    bool status = false;
    QString string;
    QPalette palette;

    status = m_port->portStatus(&string);
    palette.setColor(QPalette::WindowText, status ? Qt::darkGreen : Qt::red);
    m_portInfoLabel->setText(string);
    m_portInfoLabel->setPalette(palette);

}

// 秒定时器溢出槽函数
void MainWindow::onSecTimerTimeout()
{
    QString str;

    // 更新显示信息
    str = "RX: " + QString::number(m_controller->receiveCount()) + "Bytes";
    m_rxCntLabel->setText(str);
    str = "TX: " + QString::number(m_controller->transmitCount()) + "Bytes";
    m_txCntLabel->setText(str);

    QString label = m_portInfoLabel->text();
    if(label.contains("CLOSED")){
        if(m_port->isOpen()){
            ui->portRunAction->setEnabled(true);
            m_controller->setEnabled(m_runFlag);
            dispPortStatus(); // 更新端口状态显示
                qDebug() << "onSecTimerTimeout(), port isOpen but label shows closed";
        }
    }

}

// 打开端口
void MainWindow::openPort()
{
    if (m_port->open()) {
        QIcon icon(":/SerialTool/images/close.ico");            
        ui->portSwitchAction->setIcon(icon);
        ui->portSwitchAction->setText(tr("Close Port"));

//        QIcon icon2(":/SerialTool/images/connectyellow.ico");
        ui->portSwitchAction2->setIcon(icon);
        ui->portSwitchAction2->setText(tr("Manual Restart Port"));

        ui->portRunAction->setEnabled(true);
        m_controller->setEnabled(m_runFlag);
        dispPortStatus(); // 更新端口状态显示
    }
}

void MainWindow::closePort()
{
    if (m_port->isOpen()) {
        m_port->close();
    }
    QIcon icon(":/SerialTool/images/connect.ico");
    ui->portSwitchAction->setIcon(icon);
    ui->portSwitchAction->setText(tr("Open Port"));

    QIcon icon2(":/SerialTool/images/start.ico");
    ui->portSwitchAction2->setIcon(icon2);
    ui->portSwitchAction2->setText(tr("Auto Restart Port"));

    ui->portRunAction->setEnabled(false);
    m_controller->setEnabled(false);
    dispPortStatus(); // 更新端口状态显示
}

// 打开串口槽函数
void MainWindow::onPortSwitchActionTriggered()
{
    if (ui->portRunAction->isEnabled() == true) { // 现在需要关闭端口
        closePort();
    } else { // 端口关闭时打开端口
        openPort();
    }
m_port->autoOpen(false);
}


// 打开串口槽函数
void MainWindow::onPortSwitchAction2Triggered()
{
    if (ui->portRunAction->isEnabled() == true) { // 现在需要关闭端口
//        closePort();

        QIcon icon(":/SerialTool/images/connect.ico");

        ui->portSwitchAction2->setIcon(icon);
        ui->portSwitchAction2->setText(tr("Auto Restart Port"));

        dispPortStatus(); // 更新端口状态显示
        m_port->autoOpen(false);

    } else { // 端口关闭时打开端口
        openPort();
          qDebug() <<  (m_port->autoOpen(true)) ;
    }
}


void MainWindow::clear()
{
    m_controller->clear();
}

// 删除配置文件并退出
void MainWindow::removeConfig(){

//    QString configPath(QStandardPaths::writableLocation(
//        QStandardPaths::AppConfigLocation) + "/config.ini");
//    syncDefaultConfig(configPath);

//    QFile file(configPath);
//    file.open(QFile::WriteOnly|QFile::Truncate);
//    file.close();

    QString fileName = QCoreApplication::applicationDirPath() + "/config.ini";
    QFile fileTemp(fileName);
    fileTemp.remove();
    QApplication* app;
    app->exit(0);
}

void MainWindow::setWindowStaysOnTop(bool enabled)
{
    QString str = enabled ? ":/SerialTool/images/pin_down.ico"
                          : ":/SerialTool/images/pin_up.ico";

    ui->actionStaysOnTop->setIcon(QIcon(str));
    if (enabled) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    show();
}

void MainWindow::onStaysOnTopTriggered()
{
    setWindowStaysOnTop(!(windowFlags() & Qt::WindowStaysOnTopHint));
}

void MainWindow::about()
{

    AboutBox aboutBox(this);

    aboutBox.exec();
}

void MainWindow::openWiki()
{
    QDesktopServices::openUrl(QUrl("https://github.com/gztss/SerialTool/wiki"));
}

void MainWindow::updateSoftware()
{
    UpdateDialog *dialog = new UpdateDialog(this);

    dialog->exec();
    dialog->deleteLater();
}
