#include "terminalview.h"
#include <QSettings>
#include <QTextCodec>
#include <QTimer>
#include <QKeyEvent>
#include "ui_terminalview.h"

TerminalView::TerminalView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerminalView)
{
    ui->setupUi(this);

    resendTimer = new QTimer;
    asciiBuf = new QByteArray;

    ui->textEditRx->setReadOnly(true);
    ui->textEditTx->setWrap(true); // Send area auto wrap.

    // Send and Receive area High Light.
    ui->textEditTx->setHighLight(true);
    ui->textEditRx->setHighLight(true);

    connect(ui->wrapLineBox, SIGNAL(stateChanged(int)), this, SLOT(onWrapBoxChanged(int)));
    connect(ui->sendButton, &QPushButton::clicked, this, &TerminalView::onSendButtonClicked);
    connect(ui->resendBox, &QCheckBox::stateChanged, this, &TerminalView::updateResendTimerStatus);
    QObject::connect(resendTimer, &QTimer::timeout, this, &TerminalView::sendData);
    connect(ui->resendIntervalBox, SIGNAL(valueChanged(int)), this, SLOT(setResendInterval(int)));
    connect(ui->historyBox, SIGNAL(activated(const QString &)), this, SLOT(onHistoryBoxChanged(const QString &)));
    connect(ui->wrapLineBox, SIGNAL(stateChanged(int)), this, SLOT(onWrapBoxChanged(int)));
}

TerminalView::~TerminalView()
{
    delete ui;
    delete resendTimer;
    delete asciiBuf;
}

void TerminalView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        if (event->key() == Qt::Key_Return) {
            onSendButtonClicked(); // Ctrl + Enter to send.
        }
    }
}

void TerminalView::retranslate()
{
    ui->retranslateUi(this);
}

// load terminal config
void TerminalView::loadConfig(QSettings *config)
{
    bool status;
    QRadioButton *rbtn;

    config->beginGroup("Terminal");

    // set tx/rx mode
    rbtn = config->value("ReceiveMode").toString() == "Hex" ? ui->portReadHex : ui->portReadAscii;
    rbtn->setChecked(true);
    rbtn = config->value("TransmitMode").toString() == "Hex" ? ui->portWriteHex : ui->portWriteAscii;
    rbtn->setChecked(true);

    // set resend
    int msc = config->value("RepeatInterval").toInt();
    msc = msc < 10 ? 1000 : msc;
    ui->resendIntervalBox->setValue(msc);
    ui->resendBox->setChecked(config->value("ResendMode").toBool());
    setResendInterval(msc);
    updateResendTimerStatus();

    // set warp line
    status = config->value("RxAreaWrapLine").toBool();
    ui->wrapLineBox->setChecked(status);
    ui->textEditRx->setWrap(status);

    // load history
    loadHistory(config);

    config->endGroup();
}

void TerminalView::saveConfig(QSettings *config)
{
    QString str;

    config->beginGroup("Terminal");

    // save tx/rx mode
    str = ui->portReadHex->isChecked() ? "Hex" : "Ascii";
    config->setValue("ReceiveMode", QVariant(str));
    str = ui->portWriteHex->isChecked() ? "Hex" : "Ascii";
    config->setValue("TransmitMode", QVariant(str));
    // save resend mode
    config->setValue("RepeatInterval", QVariant(ui->resendIntervalBox->value()));
    config->setValue("ResendMode", QVariant(ui->resendBox->isChecked()));
    // save warp line
    config->setValue("RxAreaWrapLine", QVariant(ui->wrapLineBox->isChecked()));

    // save history
    saveHistory(config);

    config->endGroup();
}

void TerminalView::loadHistory(QSettings *config)
{
    config->beginGroup("HistoryBox");
    int count = config->beginReadArray("Items");
    for (int i = 0; i < count; ++i) {
        config->setArrayIndex(i);
        ui->historyBox->addItem(config->value("data").toString());
    }
    config->endArray();
    ui->historyBox->setCurrentIndex(0);
    config->endGroup();
}

void TerminalView::saveHistory(QSettings *config)
{
    config->beginGroup("HistoryBox");
    int count = ui->historyBox->count();
    config->beginWriteArray("Items");
    for (int i = 0; i < count; ++i) {
        config->setArrayIndex(i);
        config->setValue("data", ui->historyBox->itemText(i));
    }
    config->endArray();
    config->endGroup();
}

void TerminalView::append(const QByteArray &array)
{
    QString string;

    if (ui->portReadAscii->isChecked()) {
        arrayToAscii(string, array);
    } else {
        arrayToHex(string, array, 16);
    }
    ui->textEditRx->append(string);
}

void TerminalView::clear()
{
    ui->textEditRx->clear();
    asciiBuf->clear();
}

void TerminalView::setFontFamily(QString fontFamily, int size, QString style)
{
    ui->textEditRx->setFonts(fontFamily, size, Qt::black, style);
    ui->textEditTx->setFonts(fontFamily, size, Qt::black, style);
}

void TerminalView::setPaused(bool status)
{
    paused = status;
    setSendButtonEnabled(!paused && sendEnabled);
    updateResendTimerStatus();
}

void TerminalView::setSendButtonEnabled(bool status)
{
    QString str;

    ui->sendButton->setEnabled(status);
    str = status ? tr("Ctrl + Enter to send") :
        tr("Connect port and start transmission to enable this button");
    ui->sendButton->setToolTip(str);
}

void TerminalView::setEnabled(bool status)
{
    sendEnabled = status;
    setSendButtonEnabled(!paused && sendEnabled);
    // auto resend
    updateResendTimerStatus();
}

// send data
void TerminalView::sendData()
{
    QByteArray array;

    if (ui->portWriteAscii->isChecked() == true) {
        QTextCodec *code = QTextCodec::codecForName("GB-2312");
        array = code->fromUnicode(ui->textEditTx->text());
    } else {
        array = QByteArray::fromHex(ui->textEditTx->text().toLatin1());
    }
    sendDataRequest(array);
}

void TerminalView::onWrapBoxChanged(int status)
{
    ui->textEditRx->setWrap(status);
}

void TerminalView::onSendButtonClicked()
{
    QString str = ui->textEditTx->text();
    if (!str.isEmpty()) {
        sendData();

        // 历史记录下拉列表删除多余项
        while (ui->historyBox->count() >= 20) {
            ui->historyBox->removeItem(19);
        }
        // 数据写入历史记录下拉列表
        int i = ui->historyBox->findText(str);
        if (i != -1) { // 存在的项先删除
            ui->historyBox->removeItem(i);
        }
        ui->historyBox->insertItem(0, str); // 数据添加到第0个元素
        ui->historyBox->setCurrentIndex(0);
    }
}

void TerminalView::updateResendTimerStatus()
{
    bool status = sendEnabled && !paused && ui->resendBox->isChecked();

    if (status) {
        resendTimer->start(ui->resendIntervalBox->text().toInt());
    } else {
        resendTimer->stop();
    }
}

//  set resend interval time
void TerminalView::setResendInterval(int msc)
{
    resendTimer->setInterval(msc);
}

void TerminalView::onHistoryBoxChanged(const QString &string)
{
    ui->textEditTx->setText(string);
}

void TerminalView::arrayToHex(QString &str, const QByteArray &array, int countOfLine)
{
    static int count;
    int len = array.length();
    str.resize(len * 3 + (len + count) / countOfLine);
    for (int i = 0, j = 0; i < len; ++i) {
        quint8 outChar = array[i], t;   //每字节填充一次，直到结束
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
void TerminalView::arrayToAscii(QString &str, const QByteArray &array)
{
    int cnt = 0;
    int lastIndex = asciiBuf->length() - 1;

    asciiBuf->append(array);
    for (int i = lastIndex; i >= 0 && asciiBuf->at(i) & 0x80; --i) {
        cnt++;
    }
    if (cnt & 1) { // 字符串最末尾的非ASCII字节数不为2的整数倍
        char ch = asciiBuf->at(lastIndex);
        asciiBuf->remove(lastIndex, 1);
        str = QString::fromLocal8Bit(*asciiBuf);
        asciiBuf->clear();
        asciiBuf->append(ch);
    } else {
        str = QString::fromLocal8Bit(*asciiBuf);
        asciiBuf->clear();
    }
}

// 保存文件
void TerminalView::saveText(const QString &fname)
{
    QFile file(fname);

    if (file.open(QFile::WriteOnly)) {
        file.write(ui->textEditRx->text().toUtf8());
        file.close();
    }
}
