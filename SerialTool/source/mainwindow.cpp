#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include <QFileDialog>
#include <QDesktopServices>
#include "portsetbox.h"
#include "optionsbox.h"
#include "aboutbox.h"
#include "version.h"
#include "tcpudpport.h"
#include "defaultconfig.h"
#include "serialport.h"
#include "valuedisplay.h"
#include "vediobox.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString configPath(QStandardPaths::writableLocation(
        QStandardPaths::AppConfigLocation) + "/config.ini");
    syncDefaultConfig(configPath);
    m_config = new QSettings(configPath, QSettings::IniFormat);

    ui->setupUi(this);
    setWindowTitle(SOFTWARE_NAME " V" SOFTWARE_VERSION);

    // 互斥动作
    m_tabActionGroup = new QActionGroup(this);
    m_tabActionGroup->addAction(ui->actionVisibleTab0);
    m_tabActionGroup->addAction(ui->actionVisibleTab1);
    m_tabActionGroup->addAction(ui->actionVisibleTab2);

    m_serialPort = new SerialPort(this);
    m_tcpUdpPort = new TcpUdpPort(this);
    ui->toolBar1->insertWidget(ui->portSetAction, m_tcpUdpPort);
    ui->toolBar1->insertWidget(ui->portSetAction, m_serialPort);

    m_rxCount = 0;
    m_txCount = 0;
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
    connect(ui->terminal, &TerminalView::sendDataRequest, this, &MainWindow::writePort);
    connect(ui->clearAction, SIGNAL(triggered()), this, SLOT(cleanData()));
    QObject::connect(&m_timer, &QTimer::timeout, this, &MainWindow::onSecTimerTimeout);
    connect(ui->portSetAction, SIGNAL(triggered()), this, SLOT(openSetPortInfoBox()));
    connect(ui->actionOption, SIGNAL(triggered()), this, SLOT(setOptions()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionVisibleToolbar, SIGNAL(triggered(bool)), ui->toolBar1, SLOT(setVisible(bool)));
    connect(ui->actionVisibleStatusBar, SIGNAL(triggered(bool)), ui->statusBar, SLOT(setVisible(bool)));
    connect(ui->toolBar1, SIGNAL(visibilityChanged(bool)), ui->actionVisibleToolbar, SLOT(setChecked(bool)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabIndexChanged(int)));
    connect(m_tabActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(tabActionGroupTriggered(QAction*)));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionWiki, SIGNAL(triggered()), this, SLOT(openWiki()));
    connect(ui->fileTransmit, &FileTransmitView::sendData, this, &MainWindow::writePort);
    connect(ui->actionVedioBox, SIGNAL(triggered()), this, SLOT(onVedioBoxTriggered()));
    connect(ui->actionValueDisplay, SIGNAL(triggered()), this, SLOT(onValueDisplayTriggered()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    connect(m_serialPort, SIGNAL(portChanged()), this, SLOT(dispPortStatus()));
    connect(m_serialPort, SIGNAL(portError()), this, SLOT(closePort()));
    connect(m_tcpUdpPort, SIGNAL(protocolChanged()), this, SLOT(dispPortStatus()));
    connect(m_tcpUdpPort, SIGNAL(connectionError()), this, SLOT(closePort()));
    connect(ui->actionStaysOnTop, SIGNAL(triggered()), this, SLOT(onStaysOnTopTriggered()));

    m_timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_config;
    delete m_tabActionGroup;
    delete m_tcpUdpPort;
    delete m_rxCntLabel;
    delete m_txCntLabel;
    delete m_portInfoLabel;
    delete m_serialPort;
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
    ui->terminal->retranslate();
    ui->oscPlot->retranslate();
    ui->fileTransmit->retranslate();
    m_tcpUdpPort->retranslate();
    m_serialPort->retranslate();
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

    QString fontFamily("'"
        + m_config->value("FontFamily").toString().replace("+", "','") + "'");
    QString fontStyle(m_config->value("FontStyle").toString());
    int fontSize = m_config->value("FontSize").toInt();
    fontSize = fontSize < 6 ? 10 : fontSize;
    ui->terminal->setFontFamily(fontFamily, fontSize, fontStyle);

    ui->oscPlot->setBackground(QColor(m_config->value("PlotBackground").toString()));
    ui->oscPlot->setGridColor(QColor(m_config->value("AxisColor").toString()));
    // 绘制使用OpenGL加速
    ui->oscPlot->setUseOpenGL(m_config->value("UseOpenGL").toBool());
    // 绘制时抗锯齿
    ui->oscPlot->setUseAntialiased(m_config->value("UseAntialias").toBool());
    // 示波器刷新速度
    ui->oscPlot->setUpdateInterval(m_config->value("UpdateInterval").toInt());
    PortType type = (PortType)m_config->value("PortType").toInt();

    // 窗口透明度
    int opacity = m_config->value("WindowOpacity").toInt();
    opacity = opacity < 30 ? 100 : opacity <= 100 ? opacity : 100;
    setWindowOpacity(opacity / 100.0);
    if (m_valueDisplay != NULL) {
        m_valueDisplay->setWindowOpacity(windowOpacity());
    }
    if (m_vedioBox != NULL) {
        m_vedioBox->setWindowOpacity(windowOpacity());
    }

    // highlight
    ui->terminal->setHighlight(m_config->value("TerminalHighlight").toString());
    // text codec
    ui->terminal->setTextCodec(m_config->value("TerminalTextCodec").toString());

    // 语言设置
    setLanguage(m_config->value("Language").toString());
    setStyleSheet(m_config->value("Theme").toString());

    m_config->endGroup();

    if (type != m_portType) {
        closePort(); // 端口改变时关闭之前的端口
        m_portType = type;
    }
    dispPortStatus(); // 更新端口状态显示
    loadPortTool();
}

// 控件数据初始化, 在构造函数中初始化各种控件的初始值
void MainWindow::loadConfig()
{
    // 路经
    m_config->beginGroup("Path");
    m_docPath = m_config->value("DocumentPath").toString();
    m_config->endGroup();

    // 串口设置
    m_serialPort->loadConfig(m_config);

    // TCP/UDP设置
    m_tcpUdpPort->loadConfig(m_config);

    // 打开页面配置
    m_config->beginGroup("Workspace");
    ui->tabWidget->setCurrentIndex(m_config->value("TabIndex").toInt());
    setTabActionIndex(ui->tabWidget->currentIndex());
    ui->toolBar1->setVisible(m_config->value("ToolBarVisible").toBool());
    ui->actionVisibleToolbar->setChecked(m_config->value("ToolBarVisible").toBool());
    ui->statusBar->setVisible(m_config->value("StatusBarVisible").toBool());
    // 这里如果直接速读取ui->statusBar->isVisible()会是false,原因不明
    ui->actionVisibleStatusBar->setChecked(m_config->value("StatusBarVisible").toBool());
    setWindowStaysOnTop(m_config->value("WindowStaysOnTop").toBool());
    m_config->endGroup();

    // 调试终端配置
    ui->terminal->loadConfig(m_config);

    // 串口示波器
    ui->oscPlot->loadConfig(m_config);

    // 读取文件传输功能的设置
    ui->fileTransmit->loadConfig(m_config);

    // 最后读取系统设置
    loadSettings();
}

// 保存配置
void MainWindow::saveConfig()
{
    // 保存串口设置
    m_serialPort->saveConfig(m_config);

    // 保存TCP/UDP设置
    m_tcpUdpPort->saveConfig(m_config);
    
    // 打开页面配置
    m_config->beginGroup("Workspace");
    m_config->setValue("TabIndex",
        QVariant(ui->tabWidget->currentIndex()));
    m_config->setValue("ToolBarVisible", QVariant(ui->toolBar1->isVisible()));
    m_config->setValue("StatusBarVisible", QVariant(ui->statusBar->isVisible()));
    m_config->setValue("WindowStaysOnTop", QVariant((windowFlags() & Qt::WindowStaysOnTopHint) != 0));
    m_config->endGroup();

    // 调试终端配置
    ui->terminal->saveConfig(m_config);
    // 串口示波器
    ui->oscPlot->saveConfig(m_config);

    // 路经
    m_config->beginGroup("Path");
    m_config->setValue("DocumentPath", QVariant(m_docPath));
    m_config->endGroup();

    // 文件传输配置
    ui->fileTransmit->saveConfig(m_config);
}

void MainWindow::setOptions()
{
    OptionsBox option(this);
    option.exec();
}

// 保存文件
void MainWindow::saveFile()
{
    QString filter;
    QString fname = QFileDialog::getSaveFileName(this, tr("Save"), m_docPath,
        tr("Portable Network Graphic Format (*.png)") + ";;" +
        tr("Bitmap (*.bmp)") + ";;" +
        tr("Wave Plain Text File (*.txt)") + ";;" +
        tr("Terminal Text File (*.txt)"), &filter,
        QFileDialog::HideNameFilterDetails);
    if (fname.isNull()) {
        return;
    }
    m_docPath = QFileInfo(fname).path();
    if (filter == tr("Portable Network Graphic Format (*.png)")) {
        ui->oscPlot->savePng(fname);
    } else if (filter == tr("Bitmap (*.bmp)")) {
        ui->oscPlot->saveBmp(fname);
    } else if (filter == tr("Wave Plain Text File (*.txt)")) {
        ui->oscPlot->saveWave(fname);
    } else if (filter == tr("Terminal Text File (*.txt)")) {
        ui->terminal->saveText(fname);
    }
}

// 打开文件
void MainWindow::openFile()
{
    QString filter;
    QString fname = QFileDialog::getOpenFileName(this, tr("Open"), m_docPath,
        tr("Wave Plain Text File (*.txt)"), &filter,
        QFileDialog::HideNameFilterDetails);
    if (fname.isNull()) {
        return;
    }
    m_docPath = QFileInfo(fname).path();
    if (filter.indexOf("(*.txt)", 0) != -1) {
        ui->oscPlot->loadWave(fname);
    }
}

void MainWindow::tabIndexChanged(int index)
{
    setTabActionIndex(index);
    if (index == 0) { // terminal
        if (m_runFlag) { // 暂停按钮没有按下时继续发送
            ui->terminal->setPaused(false);
        }
    } else { // 其他选项卡下暂停发送
        ui->terminal->setPaused(true);
    }
}

void MainWindow::setTabActionIndex(int index)
{
    switch (index) {
    case 0:
        ui->actionVisibleTab0->setChecked(true);
        break;
    case 1:
        ui->actionVisibleTab1->setChecked(true);
        break;
    case 2:
        ui->actionVisibleTab2->setChecked(true);
        break;
    default:
        ui->actionVisibleTab0->setChecked(true);
        break;
    }
}

void MainWindow::tabActionGroupTriggered(QAction *action)
{
    if (action == ui->actionVisibleTab0) {
        ui->tabWidget->setCurrentIndex(0);
    } else if (action == ui->actionVisibleTab1) {
        ui->tabWidget->setCurrentIndex(1);
    } else if (action == ui->actionVisibleTab2) {
        ui->tabWidget->setCurrentIndex(2);
    }
}

void MainWindow::changeRunFlag()
{
    if (m_runFlag == true) {
        m_runFlag = false;
        QIcon icon(":/SerialTool/images/start.ico");
        ui->portRunAction->setIcon(icon);
        ui->portRunAction->setText(tr("Start Tx/Rx"));
        ui->terminal->setPaused(true); // terminal transmission pause
        ui->oscPlot->stop();
    } else {
        m_runFlag = true;
        QIcon icon(":/SerialTool/images/pause.ico");
        ui->portRunAction->setIcon(icon);
        ui->portRunAction->setText(tr("Pause Tx/Rx"));
        ui->terminal->setPaused(false); // terminal transmission continue
        if (ui->tabWidget->widget(ui->tabWidget->currentIndex()) == ui->tabOsc) {
            ui->oscPlot->start();
        }
    }
}

// 状态栏显示端口状态
void MainWindow::dispPortStatus()
{
    bool status = false;
    QString string;
    QPalette palette;

    if (m_portType == ComPort) {
        status = m_serialPort->portStatus(string);
    } else if (m_portType == NetworkPort) {
        status = m_tcpUdpPort->portStatus(string);
    }
    palette.setColor(QPalette::WindowText, status ? Qt::darkGreen : Qt::red);
    m_portInfoLabel->setText(string);
    m_portInfoLabel->setPalette(palette);
}

// 秒定时器溢出槽函数
void MainWindow::onSecTimerTimeout()
{
    QString str;

    // 更新显示信息
    str = "RX: " + QString::number(m_rxCount) + "Bytes";
    m_rxCntLabel->setText(str);
    str = "TX: " + QString::number(m_txCount) + "Bytes";
    m_txCntLabel->setText(str);
}

// 打开串口端口
bool MainWindow::openComPort()
{
    if (m_serialPort->open()) {
        connect(m_serialPort, &SerialPort::readyRead, this, &MainWindow::readPortData);
        return true;
    }
    return false;
}

// 打开TCP/UDP端口
bool MainWindow::openTcpUdpPort()
{
    if (m_tcpUdpPort->open()) {
        connect(m_tcpUdpPort, &TcpUdpPort::readyRead, this, &MainWindow::readPortData);
        return true;
    }
    return false;
}

// 打开端口
void MainWindow::openPort()
{
    bool status = false;

    if (m_portType == ComPort) {
        status = openComPort();
    } else if (m_portType == NetworkPort) {
        status = openTcpUdpPort();
    }
    if (status) {
        QIcon icon(":/SerialTool/images/close.ico");
        ui->portSwitchAction->setIcon(icon);
        ui->portSwitchAction->setText(tr("Close Port"));
        ui->terminal->setEnabled(true);
        ui->portRunAction->setEnabled(true);
        if (m_runFlag && ui->tabWidget->widget(ui->tabWidget->currentIndex()) == ui->tabOsc) {
            ui->oscPlot->start(); // 启动串口示波器
        }
        dispPortStatus(); // 更新端口状态显示
    }
}

void MainWindow::closePort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close(); // 关闭串口
    }
    if (m_tcpUdpPort->isOpen()) {
        m_tcpUdpPort->close();
    }
    ui->oscPlot->stop(); // 串口示波器结束运行
    QIcon icon(":/SerialTool/images/connect.ico");
    ui->portSwitchAction->setIcon(icon);
    ui->portSwitchAction->setText(tr("Open Port"));
    ui->terminal->setEnabled(false);
    ui->portRunAction->setEnabled(false);
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
}

// 打开串口设置对话框
void MainWindow::openSetPortInfoBox()
{
    m_serialPort->portSetDialog();
}

//读取接收到的数据  
void MainWindow::readPortData()
{
    // 读取串口数据
    if (!m_runFlag) {
        return;
    }
    QByteArray buf;
    if (m_portType == ComPort) {
        buf = m_serialPort->readAll(); // 读取串口数据
    } else if (m_portType == NetworkPort) {
        buf = m_tcpUdpPort->readAll(); // 读取TCP/UDP数据
    }
    m_rxCount += buf.length(); // 接收计数
    if (!buf.isEmpty()) {
        if (ui->tabWidget->currentIndex() == 0) { // 串口调试助手
            ui->terminal->append(buf);
        }
        // 串口示波器接收数据
        if (ui->tabWidget->currentIndex() == 1 || ui->oscPlot->holdReceive()) {
            ui->oscPlot->append(buf);
        }
        // 串口文件传输
        if (ui->tabWidget->currentIndex() == 2) {
            ui->fileTransmit->readData(buf);
        }
        if (m_vedioBox != NULL) {
            m_vedioBox->append(buf);
        }
        if (m_valueDisplay != NULL) {
            m_valueDisplay->append(buf);
        }
    }
    buf.clear();
}

// 向端口发送数据,带参数
void MainWindow::writePort(const QByteArray &array)
{
    m_txCount += array.length(); // 发送计数
    if (m_portType == ComPort) {
        m_serialPort->write(array);
    } else if (m_portType == NetworkPort) {
        m_tcpUdpPort->write(array);
    }
}

void MainWindow::cleanData()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0: // 串口调试助手
        ui->terminal->clear();
        break;
    case 1:
        ui->oscPlot->clear();
        break;
    }
    // 计数清零
    m_rxCount = 0;
    m_txCount = 0;
}

void MainWindow::about()
{
    AboutBox aboutBox(this);

    aboutBox.exec();
}

void MainWindow::onValueDisplayTriggered()
{
    if (m_valueDisplay == NULL) {
        m_valueDisplay = new ValueDisplay(this);
        m_valueDisplay->setModal(false);
        m_valueDisplay->setAttribute(Qt::WA_DeleteOnClose);
        m_valueDisplay->setWindowOpacity(windowOpacity());
        connect(m_valueDisplay, SIGNAL(destroyed()), this, SLOT(onValueDisplayDelete()));
        m_valueDisplay->show();
    }
}

void MainWindow::onValueDisplayDelete()
{
    m_valueDisplay = NULL;
}

void MainWindow::loadPortTool()
{
    if (m_portType == ComPort) {
        m_serialPort->setVisibleWidget(true);
        ui->portSetAction->setVisible(true);
        m_tcpUdpPort->setVisibleWidget(false);
    } else if (m_portType == NetworkPort) {
        m_serialPort->setVisibleWidget(false);
        ui->portSetAction->setVisible(false);
        m_tcpUdpPort->setVisibleWidget(true);
    }
}

void MainWindow::onVedioBoxTriggered()
{
    if (m_vedioBox == NULL) { // 当对话框没有创建时创建
        m_vedioBox = new VedioBox(this);
        connect(m_vedioBox, SIGNAL(destroyed()), this, SLOT(onVedioBoxDelete()));
        m_vedioBox->setModal(false); // 非模态对话框
        m_vedioBox->setAttribute(Qt::WA_DeleteOnClose);
        m_vedioBox->setWindowOpacity(windowOpacity());
        m_vedioBox->setFilePath(m_docPath);
        m_vedioBox->show();
    }
}

void MainWindow::onVedioBoxDelete()
{
    m_vedioBox = NULL;
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

void MainWindow::openWiki()
{
    QDesktopServices::openUrl(QUrl("https://github.com/gztss/SerialTool/wiki"));
}

void MainWindow::currentTabChanged(int index)
{
    if (m_runFlag && ui->tabWidget->widget(index) == ui->tabOsc) { // 只有在串口示波器选项卡下面才会启动示波器
        ui->oscPlot->start();
    } else {
        ui->oscPlot->stop();
    }
}
