#include "serialtool.h"
#include "portsetbox.h"
#include "optionsbox.h"
#include "aboutbox.h"
#include "wavedecode.h"
#include "version.h"
#include "tcpudpport.h"
#include "defaultconfig.h"
#include "serialport.h"

SerialTool::SerialTool(QWidget *parent)
    : QMainWindow(parent)
{
    QString configPath(QStandardPaths::writableLocation(
        QStandardPaths::AppConfigLocation) + "/config.ini");
    syncDefaultConfig(configPath);
    config = new QSettings(configPath, QSettings::IniFormat);

    ui.setupUi(this);
    setWindowTitle(SOFTWARE_NAME " V" SOFTWARE_VERSION);

    // 串口示波器波形解码器
    waveDecode = new WaveDecode();

    // 互斥动作
    tabActionGroup = new QActionGroup(this);
    tabActionGroup->addAction(ui.actionVisibleTab0);
    tabActionGroup->addAction(ui.actionVisibleTab1);
    tabActionGroup->addAction(ui.actionVisibleTab2);

    serialPort = new SerialPort(this);
    tcpUdpPort = new TcpUdpPort(this);
    ui.toolBar1->insertWidget(ui.portSetAction, tcpUdpPort);
    ui.toolBar1->insertWidget(ui.portSetAction, serialPort);

    rxCount = 0;
    txCount = 0;
    // 状态栏设置
    rxCntLabel = new QLabel("RX: 0Bytes", this);
    txCntLabel = new QLabel("TX: 0Bytes", this);
    portInfoLabel = new QLabel("", this);
    rxCntLabel->setMinimumWidth(120);
    txCntLabel->setMinimumWidth(120);
    portInfoLabel->setMinimumWidth(120);
    ui.statusBar->addWidget(portInfoLabel);
    ui.statusBar->addWidget(rxCntLabel);
    ui.statusBar->addWidget(txCntLabel);

    loadConfig(); // 加载配置
    
    // create connection between axes and scroll bars:
    connect(ui.portRunAction, SIGNAL(triggered()), this, SLOT(changeRunFlag()));
    connect(ui.portSwitchAction, SIGNAL(triggered()), this, SLOT(onPortSwitchActionTriggered()));
    connect(ui.terminal, &TerminalView::sendDataRequest, this, &SerialTool::writePort);
    connect(ui.clearAction, SIGNAL(triggered()), this, SLOT(cleanData()));
    QObject::connect(&secTimer, &QTimer::timeout, this, &SerialTool::onSecTimerTimeout);
    connect(ui.portSetAction, SIGNAL(triggered()), this, SLOT(openSetPortInfoBox()));
    connect(ui.actionOption, SIGNAL(triggered()), this, SLOT(setOptions()));
    connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui.actionVisibleToolbar, SIGNAL(triggered(bool)), ui.toolBar1, SLOT(setVisible(bool)));
    connect(ui.actionVisibleStatusBar, SIGNAL(triggered(bool)), ui.statusBar, SLOT(setVisible(bool)));
    connect(ui.toolBar1, SIGNAL(visibilityChanged(bool)), ui.actionVisibleToolbar, SLOT(setChecked(bool)));
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabIndexChanged(int)));
    connect(tabActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(tabActionGroupTriggered(QAction*)));
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui.fileTransfer, &FileTransferView::sendData, this, &SerialTool::writePort);
    connect(ui.actionVedioBox, SIGNAL(triggered()), this, SLOT(onVedioBoxTriggered()));
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    connect(serialPort, SIGNAL(portChanged()), this, SLOT(dispPortStatus()));
    connect(serialPort, SIGNAL(portError()), this, SLOT(closePort()));
    connect(tcpUdpPort, SIGNAL(protocolChanged()), this, SLOT(dispPortStatus()));
    connect(tcpUdpPort, SIGNAL(connectionError()), this, SLOT(closePort()));

    secTimer.start(1000);
}

SerialTool::~SerialTool()
{
    delete config;
    delete tabActionGroup;
    delete tcpUdpPort;
    delete rxCntLabel;
    delete txCntLabel;
    delete portInfoLabel;
    delete waveDecode;
    delete serialPort;
}

// 关闭事件
#ifdef __GNUC__
void SerialTool::closeEvent(__attribute__((unused)) QCloseEvent *event)
#else
void SerialTool::closeEvent(QCloseEvent *event)
#endif
{
    saveConfig();
}

// 加载语言
void SerialTool::setLanguage(const QString &string)
{
    // 首先卸载翻译
    if (!translator.isEmpty()) {
        for (int i = 0; i < translator.size(); ++i) {
            qApp->removeTranslator(translator[i]);  // 卸载翻译环境
            delete translator[i];
        }
        translator.clear();
    }
    // 遍历文件
    QDir dir("language/" + string);
    foreach(QFileInfo mfi ,dir.entryInfoList()) {
        if (mfi.isFile() && mfi.suffix() == "qm") { // 是翻译文件
            QTranslator* ts = new QTranslator;
            ts->load(mfi.absoluteFilePath());
            qApp->installTranslator(ts);    // 安装翻译环境
            translator.append(ts);
        }
    }
    // 重新翻译界面
    ui.retranslateUi(this);
    ui.terminal->retranslate();
    ui.oscPlot->retranslate();
    ui.fileTransfer->retranslate();
    tcpUdpPort->retranslate();
    serialPort->retranslate();
}

// 加载样式表
void SerialTool::setStyleSheet(const QString &string)
{
    // 样式表
    QFile qss("themes/" + string + "/style.css");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

// 读取系统设置
void SerialTool::loadSettings()
{
    // 系统设置
    config->beginGroup("Settings");

    QString fontFamily("'"
        + config->value("FontFamily").toString().replace("+", "','") + "'");
    QString fontStyle(config->value("FontStyle").toString());
    int fontSize = config->value("FontSize").toInt();
    fontSize = fontSize < 6 ? 10 : fontSize;
    ui.terminal->setFontFamily(fontFamily, fontSize, fontStyle);

    ui.oscPlot->setBackground(QColor(config->value("PlotBackground").toString()));
    ui.oscPlot->setGridColor(QColor(config->value("AxisColor").toString()));
    // 绘制时波形抗锯齿
    ui.oscPlot->setPlotAntialiased(config->value("PlotAntialiased").toBool());
    // 绘制时网格抗锯齿
    ui.oscPlot->setGridAntialiased(config->value("GridAntialiased").toBool());
    PortType type = (PortType)config->value("PortType").toInt();

    // 语言设置
    setLanguage(config->value("Language").toString());
    setStyleSheet(config->value("Theme").toString());

    config->endGroup();

    if (type != portType) {
        closePort(); // 端口改变时关闭之前的端口
        portType = type;
    }
    dispPortStatus(); // 更新端口状态显示
    loadPortTool();
}

// 控件数据初始化, 在构造函数中初始化各种控件的初始值
void SerialTool::loadConfig()
{
    // 路经
    config->beginGroup("Path");
    docPath = config->value("DocumentPath").toString();
    config->endGroup();

    // 串口设置
    serialPort->loadConfig(config);

    // TCP/UDP设置
    tcpUdpPort->loadConfig(config);

    // 打开页面配置
    config->beginGroup("Workspace");
    ui.tabWidget->setCurrentIndex(config->value("TabIndex").toInt());
    setTabActionIndex(ui.tabWidget->currentIndex());
    ui.toolBar1->setVisible(config->value("ToolBarVisible").toBool());
    ui.actionVisibleToolbar->setChecked(ui.toolBar1->isVisible());
    ui.statusBar->setVisible(config->value("StatusBarVisible").toBool());
    // 这里如果直接速读取ui.statusBar->isVisible()会是false,原因不明
    ui.actionVisibleStatusBar->setChecked(config->value("StatusBarVisible").toBool());
    config->endGroup();

    // 调试终端配置
    ui.terminal->loadConfig(config);

    // 串口示波器
    ui.oscPlot->loadConfig(config);

    // 读取文件传输功能的设置
    ui.fileTransfer->loadConfig(config);

    // 最后读取系统设置
    loadSettings();
}

// 保存配置
void SerialTool::saveConfig()
{
    // 保存串口设置
    serialPort->saveConfig(config);

    // 保存TCP/UDP设置
    tcpUdpPort->saveConfig(config);
    
    // 打开页面配置
    config->beginGroup("Workspace");
    config->setValue("TabIndex",
        QVariant(ui.tabWidget->currentIndex()));
    config->setValue("ToolBarVisible", QVariant(ui.toolBar1->isVisible()));
    config->setValue("StatusBarVisible", QVariant(ui.statusBar->isVisible()));
    config->endGroup();

    // 调试终端配置
    ui.terminal->saveConfig(config);
    // 串口示波器
    ui.oscPlot->saveConfig(config);

    // 路经
    config->beginGroup("Path");
    config->setValue("DocumentPath", QVariant(docPath));
    config->endGroup();

    // 文件传输配置
    ui.fileTransfer->saveConfig(config);
}

void SerialTool::setOptions()
{
    OptionsBox option(this);
    option.exec();
}

// 保存文件
void SerialTool::saveFile()
{
    QString filter;
    QString fname = QFileDialog::getSaveFileName(this, "Save", docPath,
        tr("Portable Network Graphic Format (*.png);;"
           "Bitmap (*.bmp);;"
           "Portable Document Format (*.pdf);;"
           "Wave Plain Text File (*.txt)"), &filter,
        QFileDialog::HideNameFilterDetails);
    if (fname.isNull()) {
        return;
    }
    docPath = QFileInfo(fname).path();
    if (filter.indexOf("(*.png)", 0) != -1) {
        ui.oscPlot->savePng(fname);
    } else if (filter.indexOf("(*.bmp)", 0) != -1) {
        ui.oscPlot->saveBmp(fname);
    } else if (filter.indexOf("(*.pdf)", 0) != -1) {
        ui.oscPlot->savePdf(fname);
    } else if (filter.indexOf("(*.txt)", 0) != -1) {
        ui.oscPlot->saveText(fname);
    }
}

void SerialTool::tabIndexChanged(int index)
{
    setTabActionIndex(index);
    if (index == 0) { // terminal
        if (runFlag) { // 暂停按钮没有按下时继续发送
            ui.terminal->setPaused(false);
        }
    } else { // 其他选项卡下暂停发送
        ui.terminal->setPaused(true);
    }
}

void SerialTool::setTabActionIndex(int index)
{
    switch (index) {
    case 0:
        ui.actionVisibleTab0->setChecked(true);
        break;
    case 1:
        ui.actionVisibleTab1->setChecked(true);
        break;
    case 2:
        ui.actionVisibleTab2->setChecked(true);
        break;
    default:
        ui.actionVisibleTab0->setChecked(true);
        break;
    }
}

void SerialTool::tabActionGroupTriggered(QAction *action)
{
    if (action == ui.actionVisibleTab0) {
        ui.tabWidget->setCurrentIndex(0);
    } else if (action == ui.actionVisibleTab1) {
        ui.tabWidget->setCurrentIndex(1);
    } else if (action == ui.actionVisibleTab2) {
        ui.tabWidget->setCurrentIndex(2);
    }
}

void SerialTool::changeRunFlag()
{
    if (runFlag == true) {
        runFlag = false;
        QIcon icon(":/SerialTool/images/start.ico");
        ui.portRunAction->setIcon(icon);
        ui.portRunAction->setText(tr("Start Tx/Rx"));
        ui.terminal->setPaused(true); // terminal transmission pause
        ui.oscPlot->stop();
    } else {
        runFlag = true;
        QIcon icon(":/SerialTool/images/pause.ico");
        ui.portRunAction->setIcon(icon);
        ui.portRunAction->setText(tr("Pause Tx/Rx"));
        ui.terminal->setPaused(false); // terminal transmission continue
        if (ui.tabWidget->widget(ui.tabWidget->currentIndex()) == ui.tabOsc) {
            ui.oscPlot->start();
        }
    }
}

// 状态栏显示端口状态
void SerialTool::dispPortStatus()
{
    bool status = false;
    QString string;
    QPalette palette;

    if (portType == ComPort) {
        status = serialPort->portStatus(string);
    } else if (portType == NetworkPort) {
        status = tcpUdpPort->portStatus(string);
    }
    palette.setColor(QPalette::WindowText, status ? Qt::darkGreen : Qt::red);
    portInfoLabel->setText(string);
    portInfoLabel->setPalette(palette);
}

// 秒定时器溢出槽函数
void SerialTool::onSecTimerTimeout()
{
    QString str;

    // 更新显示信息
    str = "RX: " + QString::number(rxCount) + "Bytes";
    rxCntLabel->setText(str);
    str = "TX: " + QString::number(txCount) + "Bytes";
    txCntLabel->setText(str);
}

// 打开串口端口
bool SerialTool::openComPort()
{
    if (serialPort->open()) {
        connect(serialPort, &SerialPort::readyRead, this, &SerialTool::readPortData);
        return true;
    }
    return false;
}

// 打开TCP/UDP端口
bool SerialTool::openTcpUdpPort()
{
    if (tcpUdpPort->open()) {
        connect(tcpUdpPort, &TcpUdpPort::readyRead, this, &SerialTool::readPortData);
        return true;
    }
    return false;
}

// 打开端口
void SerialTool::openPort()
{
    bool status = false;

    if (portType == ComPort) {
        status = openComPort();
    } else if (portType == NetworkPort) {
        status = openTcpUdpPort();
    }
    if (status) {
        QIcon icon(":/SerialTool/images/close.png");
        ui.portSwitchAction->setIcon(icon);
        ui.portSwitchAction->setText(tr("Close Port"));
        ui.terminal->setEnabled(true);
        ui.portRunAction->setEnabled(true);
        if (runFlag && ui.tabWidget->widget(ui.tabWidget->currentIndex()) == ui.tabOsc) {
            ui.oscPlot->start(); // 启动串口示波器
        }
        dispPortStatus(); // 更新端口状态显示
    }
}

void SerialTool::closePort()
{
    if (serialPort->isOpen()) {
        serialPort->close(); // 关闭串口
    }
    if (tcpUdpPort->isOpen()) {
        tcpUdpPort->close();
    }
    ui.oscPlot->stop(); // 串口示波器结束运行
    QIcon icon(":/SerialTool/images/connect.png");
    ui.portSwitchAction->setIcon(icon);
    ui.portSwitchAction->setText(tr("Open Port"));
    ui.terminal->setEnabled(false);
    ui.portRunAction->setEnabled(false);
    dispPortStatus(); // 更新端口状态显示
}

// 打开串口槽函数
void SerialTool::onPortSwitchActionTriggered()
{
    if (ui.portRunAction->isEnabled() == true) { // 现在需要关闭端口
        closePort();
    } else { // 端口关闭时打开端口
        openPort();
    }
}

// 打开串口设置对话框
void SerialTool::openSetPortInfoBox()
{
    serialPort->portSetDialog();
}

//读取接收到的数据  
void SerialTool::readPortData()
{
    // 读取串口数据
    if (!runFlag) {
        return;
    }
    QByteArray buf;
    if (portType == ComPort) {
        buf = serialPort->readAll(); // 读取串口数据
    } else if (portType == NetworkPort) {
        buf = tcpUdpPort->readAll(); // 读取TCP/UDP数据
    }
    rxCount += buf.length(); // 接收计数
    if (!buf.isEmpty()) {
        if (ui.tabWidget->currentIndex() == 0) { // 串口调试助手
            ui.terminal->append(buf);
        }
        // 串口示波器接收数据
        if (ui.tabWidget->currentIndex() == 1 || ui.oscPlot->holdReceive()) {
            for (int i = 0; i < buf.length(); ++i) {
                WaveDataType data;
                // 解析串口协议
                if (waveDecode->frameDecode(data, buf.data()[i]) == true) {
                    ui.oscPlot->addData(data);
                }
            }
        }
        // 串口文件传输
        if (ui.tabWidget->currentIndex() == 2) {
            ui.fileTransfer->readData(buf);
        }
        if (vedioBox != NULL) {
            vedioBox->addData(buf);
        }
    }
    buf.clear();
}

// 向端口发送数据,带参数
void SerialTool::writePort(const QByteArray &array)
{
    txCount += array.length(); // 发送计数
    if (portType == ComPort) {
        serialPort->write(array);
    } else if (portType == NetworkPort) {
        tcpUdpPort->write(array);
    }
}

void SerialTool::cleanData()
{
    switch (ui.tabWidget->currentIndex()) {
    case 0: // 串口调试助手
        ui.terminal->clear();
        break;
    case 1:
        ui.oscPlot->clear();
        break;
    }
    // 计数清零
    rxCount = 0;
    txCount = 0;
}

void SerialTool::about()
{
    AboutBox aboutBox(this);
    
    aboutBox.exec();
}

void SerialTool::loadPortTool()
{
    if (portType == ComPort) {
        serialPort->setVisibleWidget(true);
        ui.portSetAction->setVisible(true);
        tcpUdpPort->setVisibleWidget(false);
    } else if (portType == NetworkPort) {
        serialPort->setVisibleWidget(false);
        ui.portSetAction->setVisible(false);
        tcpUdpPort->setVisibleWidget(true);
    }
}

void SerialTool::onVedioBoxTriggered()
{
    if (vedioBox == NULL) { // 当对话框没有创建时创建
        vedioBox = new VedioBox(this);
        connect(vedioBox, SIGNAL(destroyed()), this, SLOT(onVedioBoxDelete()));
        vedioBox->setModal(false); // 非模态对话框
        vedioBox->setAttribute(Qt::WA_DeleteOnClose);
        vedioBox->setFilePath(docPath);
        vedioBox->show();
    }
}

void SerialTool::onVedioBoxDelete()
{
    vedioBox = NULL;
}

void SerialTool::currentTabChanged(int index)
{
    if (runFlag && ui.tabWidget->widget(index) == ui.tabOsc) { // 只有在串口示波器选项卡下面才会启动示波器
        ui.oscPlot->start();
    } else {
        ui.oscPlot->stop();
    }
}
