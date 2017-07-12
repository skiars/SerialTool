#include "serialtool.h"
#include "portsetbox.h"
#include "optionsbox.h"
#include "aboutbox.h"
#include "wavedecode.h"
#include "version.h"
#include "tcpudpport.h"

SerialTool::SerialTool(QWidget *parent)
    : QMainWindow(parent)
{
    config = new QSettings("config.ini", QSettings::IniFormat);

    ui.setupUi(this);
    setWindowTitle(SOFTWARE_NAME " V" SOFTWARE_VERSION);

    // 互斥动作
    tabActionGroup = new QActionGroup(this);
    tabActionGroup->addAction(ui.actionVisibleTab0);
    tabActionGroup->addAction(ui.actionVisibleTab1);

    // 对界面的初始化结束之后再设置语言和样式表
    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);
    qApp->installTranslator(&qsciTranslator);
    setLanguage("zh_CN");
    setStyleSheet("default");

    serialPort = new QSerialPort;
    tcpUdpPort = new TcpUdpPort(this);
    // 将串口配置控件移除
    ui.verticalLayout_2->removeWidget(ui.portConfigWidget);
    ui.toolBar1->insertWidget(ui.portSetAction, ui.portConfigWidget);
    ui.toolBar1->insertWidget(ui.portSetAction, tcpUdpPort);

    rxCount = 0;
    txCount = 0;
    // 状态栏设置
    rxCntLabel = new QLabel("Rx: 0Bytes", this);
    txCntLabel = new QLabel("Tx: 0Bytes", this);
    portInfoLabel = new QLabel("", this);
    rxCntLabel->setMinimumWidth(120);
    txCntLabel->setMinimumWidth(120);
    portInfoLabel->setMinimumWidth(120);
    ui.statusBar->addWidget(portInfoLabel);
    ui.statusBar->addWidget(rxCntLabel);
    ui.statusBar->addWidget(txCntLabel);

    ui.textEditRx->setReadOnly(true);
    ui.textEditTx->setWrap(true); // 发送区自动换行

    loadConfig(); // 加载配置
    
    // create connection between axes and scroll bars:
    connect(ui.portRunAction, SIGNAL(triggered()), this, SLOT(changeRunFlag()));
    connect(ui.portSwitchAction, SIGNAL(triggered()), this, SLOT(onPortSwitchActionTriggered()));
    connect(ui.sendButton, &QPushButton::clicked, this, &SerialTool::onSendButtonClicked);
    connect(ui.clearAction, SIGNAL(triggered()), this, SLOT(cleanData()));
    QObject::connect(&resendTimer, &QTimer::timeout, this, &SerialTool::writePortData);
    QObject::connect(&secTimer, &QTimer::timeout, this, &SerialTool::onSecTimerTimeout);
    connect(ui.resendBox, &QCheckBox::stateChanged, this, &SerialTool::onResendBoxChanged);
    connect(ui.comboBoxBaudRate, &QComboBox::currentTextChanged, this, &SerialTool::setPortBaudRate);
    connect(ui.spinBoxStepTime, SIGNAL(valueChanged(int)), this, SLOT(resendTimeChange(int)));
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
    connect(ui.comboBox, SIGNAL(activated(const QString &)), this, SLOT(onComboBoxChanged(const QString &)));
    connect(ui.wrapLineBox, SIGNAL(stateChanged(int)), this, SLOT(onWrapBoxChanged(int)));
    connect(ui.fileTransfer, &FileTransferView::sendData, this, &SerialTool::writePort);
    connect(ui.actionVedioBox, SIGNAL(triggered()), this, SLOT(onVedioBoxTriggered()));
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

    scanPort(); // 扫描端口
    secTimer.start(1000);
}

SerialTool::~SerialTool()
{
    
}

// 关闭事件
void SerialTool::closeEvent(QCloseEvent *event)
{
    saveConfig();
}

// 加载语言
void SerialTool::setLanguage(const QString &string)
{
    appTranslator.load("language/" + string + ".qm");
    qtTranslator.load("language/qt_" + string + ".qm");
    qsciTranslator.load("language/qscintilla_" + string + ".qm");
    ui.retranslateUi(this);    // 重新翻译界面
    ui.oscPlot->retranslate(); // 示波器界面重新翻译
    ui.fileTransfer->retranslate(); // 文件传输界面重新翻译
}

// 加载样式表
void SerialTool::setStyleSheet(const QString &string)
{
    // 样式表
    QFile qss("themes/" + string + ".css");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

// 读取系统设置
void SerialTool::loadSettings()
{
    // 系统设置
    config->beginGroup("Settings");

    QString fonts("'" 
        + config->value("FontFamily").toString().replace("+", "','") + "'");
    QString fontStyle(config->value("FontStyle").toString());
    int fontSize = config->value("FontSize").toInt();
    ui.textEditRx->setFonts(fonts, fontSize,
        QColor(config->value("ReceiveTextColor").toString()), fontStyle);
    ui.textEditTx->setFonts(fonts, fontSize,
        QColor(config->value("TransmitTextColor").toString()), fontStyle);

    ui.oscPlot->setBackground(QColor(config->value("PlotBackground").toString()));
    ui.oscPlot->setGridColor(QColor(config->value("AxisColor").toString()));

    // 绘制时波形抗锯齿
    ui.oscPlot->setPlotAntialiased(config->value("PlotAntialiased").toBool());
    // 绘制时网格抗锯齿
    ui.oscPlot->setGridAntialiased(config->value("GridAntialiased").toBool());
    PortType type = (PortType)config->value("PortType").toInt();

    config->endGroup();

    // 发送区与接收区设置语法高亮
    ui.textEditTx->setHighLight(true);
    ui.textEditRx->setHighLight(true);

    if (type != portType) {
        closePort(); // 端口改变时关闭之前的端口
        portType = type;
    }
    loadPortTool();
}

// 控件数据初始化, 在构造函数中初始化各种控件的初始值
void SerialTool::loadConfig()
{
    // 路经
    config->beginGroup("Path");
    docPath = config->value("DocumentPath").toString();
    config->endGroup();

    // 控件数据
    config->beginGroup("WidgetData");
    config->beginGroup("comboBox");
    int count = config->value("Count").toInt();
    for (int i = 0; i < count; ++i) {
        ui.comboBox->addItem(
            config->value("Item" + QString::number(i)).toString());
    }
    ui.comboBox->setCurrentIndex(0);
    config->endGroup();
    config->endGroup();

    // 串口设置
    config->beginGroup("SerialPort");
    serialPort->setBaudRate(config->value("BaudRate").toInt());
    ui.comboBoxBaudRate->setCurrentText(QString::number(serialPort->baudRate()));
    config->endGroup();

    // TCP/UDP设置
    config->beginGroup("TcpUdpPort");
    tcpUdpPort->setServerAddress(config->value("ServerAddress").toString());
    tcpUdpPort->setPortNumber(config->value("PortNumber").toInt());
    tcpUdpPort->setPortProtocol(config->value("PortProtocol").toString());
    config->endGroup();

    // 打开页面配置
    config->beginGroup("Workspace");
    ui.tabWidget->setCurrentIndex(config->value("TabIndex").toInt());
    tabIndexChanged(ui.tabWidget->currentIndex());
    ui.toolBar1->setVisible(config->value("ToolBarVisible").toBool());
    ui.actionVisibleToolbar->setChecked(ui.toolBar1->isVisible());
    ui.statusBar->setVisible(config->value("StatusBarVisible").toBool());
    // 这里如果直接速读取ui.statusBar->isVisible()会是false,原因不明
    ui.actionVisibleStatusBar->setChecked(config->value("StatusBarVisible").toBool());
    config->endGroup();

    // 串口调试工具配置
    config->beginGroup("TestTool");
    if (config->value("ReceiveMode") == "Hex") {
        ui.portReadHex->setChecked(true);
    } else {
        ui.portReadAscii->setChecked(true);
    }
    if (config->value("TransmitMode") == "Hex") {
        ui.portWriteHex->setChecked(true);
    } else {
        ui.portWriteAscii->setChecked(true);
    }
    if (config->value("RepeatInterval").toInt() == 0) { // 自动重发时间
        ui.spinBoxStepTime->setValue(1000);
    } else {
        ui.spinBoxStepTime->setValue(config->value("RepeatInterval").toInt());
    }
    ui.resendBox->setChecked(config->value("ResendMode").toBool()); // 重复发送
    onResendBoxChanged(ui.resendBox->isChecked());
    ui.wrapLineBox->setChecked(config->value("RxAreaWrapLine").toBool()); // 接收区自动换行
    onWrapBoxChanged(ui.wrapLineBox->isChecked());
    config->endGroup();

    // 串口示波器
    config->beginGroup("Oscillograph");
    ui.oscPlot->setXRange(config->value("XRange").toString());
    ui.oscPlot->setYOffset(config->value("YOffset").toDouble());
    ui.oscPlot->setYRange(config->value("YRange").toDouble());
    for (int i = 0; i < CH_NUM; ++i) {
        ui.oscPlot->setChannelColor(i, QColor(
            config->value("Ch" + QString::number(i + 1) + "Color").toString()));
        ui.oscPlot->setChannelVisible(i,
                config->value("Ch" + QString::number(i + 1) + "Visible").toBool());
    }
    config->endGroup();

    // 读取文件传输功能的设置
    ui.fileTransfer->loadConfig(config);

    // 最后读取系统设置
    loadSettings();
}

// 保存配置
void SerialTool::saveConfig()
{
    // 保存串口设置
    config->beginGroup("SerialPort");
    config->setValue("BaudRate",
        QVariant(ui.comboBoxBaudRate->currentText()));
    config->endGroup();

    // 保存TCP/UDP设置
    config->beginGroup("TcpUdpPort");
    config->setValue("ServerAddress",
        QVariant(tcpUdpPort->serverAddress()));
    config->setValue("PortNumber",
        QVariant(tcpUdpPort->portNumber()));
    config->setValue("PortProtocol",
        QVariant(tcpUdpPort->portProtocol()));
    config->endGroup();
    
    // 打开页面配置
    config->beginGroup("Workspace");
    config->setValue("TabIndex",
        QVariant(ui.tabWidget->currentIndex()));
    config->setValue("ToolBarVisible", QVariant(ui.toolBar1->isVisible()));
    config->setValue("StatusBarVisible", QVariant(ui.statusBar->isVisible()));
    config->endGroup();

    // 串口调试工具配置
    config->beginGroup("TestTool");
    if (ui.portReadHex->isChecked()) {
        config->setValue("ReceiveMode", QVariant("Hex"));
    } else {
        config->setValue("ReceiveMode", QVariant("ASCII"));
    }
    if (ui.portWriteHex->isChecked()) {
        config->setValue("TransmitMode", QVariant("Hex"));
    } else {
        config->setValue("TransmitMode", QVariant("ASCII"));
    }
    config->setValue("RepeatInterval",
        QVariant(QString::number(ui.spinBoxStepTime->value())));
    config->setValue("ResendMode",
        QVariant(ui.resendBox->isChecked())); // 自动重发
    config->setValue("RxAreaWrapLine",
        QVariant(ui.wrapLineBox->isChecked())); // 接收区自动换行
    config->endGroup();

    // 串口示波器
    config->beginGroup("Oscillograph");
    config->setValue("YOffset", QVariant(ui.oscPlot->yOffset()));
    config->setValue("YRange", QVariant(ui.oscPlot->yRange()));
    config->setValue("XRange", QVariant(ui.oscPlot->xRange()));
    
    for (int i = 0; i < CH_NUM; ++i) {
        config->setValue("Ch" + QString::number(i + 1) + "Visible",
            QVariant(ui.oscPlot->channelVisible(i)));
        config->setValue("Ch" + QString::number(i + 1) + "Color",
                QVariant(ui.oscPlot->channelColor(i).name()));
    }
    config->endGroup();

    // 路经
    config->beginGroup("Path");
    config->setValue("DocumentPath", QVariant(docPath));
    config->endGroup();

    // 控件数据
    config->beginGroup("WidgetData");
    config->beginGroup("comboBox");
    int count = ui.comboBox->count();
    config->setValue("Count", count);
    for (int i = 0; i < count; ++i) {
        config->setValue("Item" + QString::number(i),
            QVariant(ui.comboBox->itemText(i)));
    }
    config->endGroup();
    config->endGroup();

    // 读取文件传输功能的设置
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
    if (index == 0) {
        ui.actionVisibleTab0->setChecked(true);
    } else {
        ui.actionVisibleTab1->setChecked(true);
    }
}

void SerialTool::tabActionGroupTriggered(QAction *action)
{
    if (action == ui.actionVisibleTab0) {
        ui.tabWidget->setCurrentIndex(0);
    } else {
        ui.tabWidget->setCurrentIndex(1);
    }
    
}

void SerialTool::changeRunFlag()
{
    if (runFlag == true) {
        runFlag = false;
        QIcon icon(":/SerialTool/images/start.ico");
        ui.portRunAction->setIcon(icon);
        ui.portRunAction->setText(tr("Start Tx/Rx"));
        ui.oscPlot->stop();
    } else {
        runFlag = true;
        QIcon icon(":/SerialTool/images/pause.ico");
        ui.portRunAction->setIcon(icon);
        ui.portRunAction->setText(tr("Pause Tx/Rx"));
        if (ui.tabWidget->widget(ui.tabWidget->currentIndex()) == ui.tabOsc) {
            ui.oscPlot->start();
        }
    }
}

void SerialTool::onSecTimerTimeout()
{
    const QString parity[] {
        "None", "Even", "Odd", "Space", "Mark", "Unknown"
    };
    const QString flowControl[] {
        "None", "RTS/CTS", "XON/XOFF", "Unknown"
    };

    if (serialPort->isOpen()) {
        // 检查端口是否有错误
        if (serialPort->error() != QSerialPort::NoError) {
            closePort();
            scanPort();
        }
    } else {
        scanPort(); // 串口关闭时扫描可用端口
    }
    // 更新显示信息
    QString str;
    str = ui.comboBoxPortNum->currentText().section(" ", 0, 0) + " ";
    if (serialPort->isOpen()) {
        str += "OPEND, " + QString::number(serialPort->baudRate()) + "bps, "
            + QString::number(serialPort->dataBits()) + "bit, "
            + parity[serialPort->parity()] + ", "
            + QString::number(serialPort->stopBits()) + ", "
            + flowControl[serialPort->flowControl()];
        portInfoLabel->setStyleSheet("color:green; font-family: Microsoft YaHei UI;");
    } else {
        str += "CLOSED";
        portInfoLabel->setStyleSheet("color:red; font-family: Microsoft YaHei UI;");
    }
    portInfoLabel->setText(str);
    str = "Rx: " + QString::number(rxCount) + "Bytes";
    rxCntLabel->setText(str);
    str = "Tx: " + QString::number(txCount) + "Bytes";
    txCntLabel->setText(str);
}

// 扫描端口
void SerialTool::scanPort()
{
    int i = 0;
    bool sync = false;
    QVector<QSerialPortInfo> vec;
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        // 端口名不在匹配说明端口列表变更
        if (ui.comboBoxPortNum->itemText(i++).indexOf(info.portName())) {
            sync = true;
        }
        vec.append(info);
    }
    // 需要同步或者comboBoxPortNum存在无效端口
    if (sync || !ui.comboBoxPortNum->itemText(i).isEmpty()) {
        int len = 0;
        QFontMetrics fm(ui.comboBoxPortNum->font());
        QString str = ui.comboBoxPortNum->currentText();
        ui.comboBoxPortNum->clear();
        for (i = 0; i < vec.length(); ++i) {
            QString t = vec[i].portName() + " (" + vec[i].description() + ")";
            ui.comboBoxPortNum->addItem(t);
            int width = fm.boundingRect(t).width(); // 计算字符串的像素宽度
            if (width > len) { // 统计最长字符串
                len = width;
            }
        }
        if (!str.isEmpty()) { // 设置当前选中的端口
            ui.comboBoxPortNum->setCurrentText(str);
        } else {
            ui.comboBoxPortNum->setCurrentIndex(0);
        }
        // 自动控制下拉列表宽度
        ui.comboBoxPortNum->view()->setMinimumWidth(len + 9);
    }
}

// 打开串口端口
bool SerialTool::openComPort()
{
    QString name = ui.comboBoxPortNum->currentText().section(' ', 0, 0);
    serialPort->setPortName(name);
    if (serialPort->open(QIODevice::ReadWrite)) {
        ui.comboBoxPortNum->setEnabled(false); // 禁止更改串口
        connect(serialPort, &QSerialPort::readyRead, this, &SerialTool::readPortData);
        return true;
    }
    QMessageBox err(QMessageBox::Critical,
        tr("Error"),
        tr("Can not open the port!\n"
            "Port may be occupied or configured incorrectly!"),
        QMessageBox::Cancel, this);
    err.exec();
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
        ui.sendButton->setEnabled(true);
        ui.portRunAction->setEnabled(true);
        if (runFlag && ui.tabWidget->widget(ui.tabWidget->currentIndex()) == ui.tabOsc) {
            ui.oscPlot->start(); // 启动串口示波器
        }
    }
}

void SerialTool::closePort()
{
    if (serialPort->isOpen()) {
        serialPort->close(); // 关闭串口
        ui.comboBoxPortNum->setEnabled(true); // 允许更改串口
    }
    if (tcpUdpPort->isOpen()) {
        tcpUdpPort->close();
    }
    ui.oscPlot->stop(); // 串口示波器开始结束运行
    QIcon icon(":/SerialTool/images/connect.png");
    ui.portSwitchAction->setIcon(icon);
    ui.portSwitchAction->setText(tr("Open Port"));
    ui.sendButton->setEnabled(false);
    ui.portRunAction->setEnabled(false);
}

// 打开串口槽函数
void SerialTool::onPortSwitchActionTriggered()
{ 
    bool opend = serialPort->isOpen() || tcpUdpPort->isOpen();
    if (opend == true) { // 现在需要关闭端口
        closePort();
    } else { // 端口关闭时打开端口
        openPort();
    }
}

// 打开串口设置对话框
void SerialTool::openSetPortInfoBox()
{
    PortSetBox portSet(serialPort);
    portSet.exec();
}

void SerialTool::setPortBaudRate(const QString &string)
{
    serialPort->setBaudRate(string.toInt());
}

void SerialTool::onSendButtonClicked()
{
    if (!runFlag) { // 如果用户按下发送按钮则退出暂停模式
        changeRunFlag();
    }
    QString str = ui.textEditTx->text();
    if (!str.isEmpty()) {
        writePortData();

        // 历史记录下拉列表删除多余项
        while (ui.comboBox->count() >= 20) {
            ui.comboBox->removeItem(19);
        }
        // 数据写入历史记录下拉列表
        int i = ui.comboBox->findText(str);
        if (i != -1) { // 存在的项先删除
            ui.comboBox->removeItem(i);
        }
        ui.comboBox->insertItem(0, str); // 数据添加到第0个元素
        ui.comboBox->setCurrentIndex(0);
    }
}

static void byteArrayToHex(QString &str, QByteArray &arr, int countOfLine)
{
    static int count;
    int len = arr.length();
    str.resize(len * 3 + (len + count) / countOfLine);
    for (int i = 0, j = 0; i < len; ++i) {
        quint8 outChar = arr[i], t;   //每字节填充一次，直到结束
        //十六进制的转换
        t = (outChar >> 4);
        str[j++] = t + (t < 10 ? '0' : 'A' - 10);
        t = outChar & 0x0F;
        str[j++] = t + (t < 10 ? '0' : 'A' - 10);
        str[j++] = ' ';
        if (count >= countOfLine - 1) {
            count = 0;
            str[j++] = '\n';
        } else {
            ++count;
        }
    }
}

// 这个函数可以避免中文接收的乱码
static void byteArrayToAscii(QString &str, QByteArray &arr, QByteArray &buf)
{
    int cnt = 0;

    buf += arr;
    for (int i = buf.length() - 1; i >= 0 && (quint8)(buf[i]) > 128; --i) {
        cnt++;
    }
    if (cnt & 1) { // 字符串最末尾的非ASCII字节数不为2的整数倍
        char ch = buf[buf.length() - 1];
        buf.remove(buf.length() - 1, 1);
        str = QString::fromLocal8Bit(buf);
        buf.clear();
        buf.append(ch);
    } else {
        str = QString::fromLocal8Bit(buf);
        buf.clear();
    }
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
            QString str;
            if (ui.portReadAscii->isChecked()) { // ASCII模式
                byteArrayToAscii(str, buf, asciiBuf);
            } else {
                byteArrayToHex(str, buf, 16);
            }
            ui.textEditRx->append(str);
        }
        // 串口示波器接收数据
        if (ui.tabWidget->currentIndex() == 1 || ui.oscPlot->holdReceive()) {
            for (int i = 0; i < buf.length(); ++i) {
                char ch;
                double value;
                // 解析串口协议
                if (waveGetPointValue(ch, value, buf.data()[i]) == true) {
                    ui.oscPlot->addData(ch, value);
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

// 向串口发送数据
void SerialTool::writePortData()
{
    if (runFlag && (serialPort->isOpen() || portType == NetworkPort)) {
        QByteArray arr;
        if (ui.portWriteAscii->isChecked() == true) {
            QTextCodec *code = QTextCodec::codecForName("GB-2312");
            arr = code->fromUnicode(ui.textEditTx->text());
        } else {
            arr = QByteArray::fromHex(ui.textEditTx->text().toLatin1());
        }
        writePort(arr);
    }
}

// 向端口发送数据,带参数
void SerialTool::writePort(const QByteArray &array)
{
    txCount += array.length(); // 发送计数
    if (portType == ComPort) {
        serialPort->write(array.data(), array.length());
    } else if (portType == NetworkPort) {
        tcpUdpPort->write(array);
    }
}

void SerialTool::cleanData()
{
    switch (ui.tabWidget->currentIndex()) {
    case 0: // 串口调试助手
        ui.textEditRx->clear();
        asciiBuf.clear();
        break;
    case 1:
        ui.oscPlot->clear();
        break;
    }
    // 计数清零
    rxCount = 0;
    txCount = 0;
}

void SerialTool::onResendBoxChanged(int status)
{
    if (status) {
        resendTimer.start(ui.spinBoxStepTime->text().toInt());
    } else {
        resendTimer.stop();
    }
}

//  重复发送时间修改
void SerialTool::resendTimeChange(int msc)
{
    resendTimer.setInterval(msc);
}

void SerialTool::about()
{
    AboutBox aboutBox(this);
    
    aboutBox.exec();
}

void SerialTool::onComboBoxChanged(const QString &string)
{
    ui.textEditTx->setText(string);
}

void SerialTool::onWrapBoxChanged(int status)
{
    ui.textEditRx->setWrap(status);
}

void SerialTool::loadPortTool()
{
    if (portType == ComPort) {
        ui.portConfigWidget->setVisible(true);
        ui.label->setVisible(true);
        ui.comboBoxPortNum->setVisible(true);
        ui.label_2->setVisible(true);
        ui.comboBoxBaudRate->setVisible(true);
        ui.portConfigWidget->setVisible(true);
        ui.portSetAction->setVisible(true);
        tcpUdpPort->setVisibleWidget(false);
    } else if (portType == NetworkPort) {
        ui.portConfigWidget->setVisible(false);
        ui.label->setVisible(false);
        ui.comboBoxPortNum->setVisible(false);
        ui.label_2->setVisible(false);
        ui.comboBoxBaudRate->setVisible(false);
        ui.portConfigWidget->setVisible(false);
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
