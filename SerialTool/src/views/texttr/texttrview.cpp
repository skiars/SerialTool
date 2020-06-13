#include "texttrview.h"
#include "ui_texttrview.h"
#include <QSettings>
#include <QTextCodec>
#include <QTimer>
#include <QKeyEvent>
#include <QDateTime>

TextTRView::TextTRView(QWidget *parent) :
    AbstractView(parent),
    ui(new Ui::TextTRView)
{
    ui->setupUi(this);

    m_resendTimer = new QTimer;
    m_asciiBuf = new QByteArray;

    ui->textEditRx->setReadOnly(true);
    ui->textEditTx->setWrap(true); // Send area auto wrap.

    setTextCodec("GB-2312"); // default

    connect(ui->wrapLineBox, SIGNAL(stateChanged(int)), this, SLOT(onWrapBoxChanged(int)));
    connect(ui->sendButton, &QPushButton::clicked, this, &TextTRView::onSendButtonClicked);
    connect(ui->resendBox, &QCheckBox::stateChanged, this, &TextTRView::updateResendTimerStatus);
    QObject::connect(m_resendTimer, &QTimer::timeout, this, &TextTRView::sendData);
    connect(ui->resendIntervalBox, SIGNAL(valueChanged(int)), this, SLOT(setResendInterval(int)));
    connect(ui->historyBox, SIGNAL(activated(const QString &)), this, SLOT(onHistoryBoxChanged(const QString &)));
    connect(ui->wrapLineBox, SIGNAL(stateChanged(int)), this, SLOT(onWrapBoxChanged(int)));
}

TextTRView::~TextTRView()
{
    delete ui;
    delete m_resendTimer;
    delete m_asciiBuf;
}

void TextTRView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        if (event->key() == Qt::Key_Return) {
            onSendButtonClicked(); // Ctrl + Enter to send.
        }
    }
}

void TextTRView::retranslate()
{
    ui->retranslateUi(this);
}

// load terminal config
void TextTRView::loadConfig(QSettings *config)
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

void TextTRView::saveConfig(QSettings *config)
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

void TextTRView::loadSettings(QSettings *config)
{
    QString fontFamily(config->value("FontFamily").toString());
    QString fontStyle(config->value("FontStyle").toString());
    int fontSize = config->value("FontSize").toInt();

    fontSize = fontSize < 6 ? 10 : fontSize;
    setFontFamily(fontFamily, fontSize, fontStyle);

    // highlight
    setHighlight(config->value("TerminalHighlight").toString());
    // text codec
    setTextCodec(config->value("TerminalTextCodec").toString());
    setTabsInsertSpaces(config->value("TerminalTabsInsertSpaces").toBool());
    setTabWidth(config->value("TerminalTabWidth").toInt());
    setAutoIndent(config->value("TerminalAutoIndent").toBool());
    setIndentationGuides(config->value("TerminalIndentationGuides").toBool());
    m_timeStamp = config->value("TerminalUseTimeStamp").toBool();
    m_frameSeparator = config->value("TerminalFrameSeparator").toString();
    m_timeStampFormat = config->value("TerminalTimeStampFormat").toString();
}

void TextTRView::loadHistory(QSettings *config)
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

void TextTRView::saveHistory(QSettings *config)
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

void TextTRView::setHighlight(const QString &language)
{
    // Send and Receive area highlight.
    ui->textEditTx->setHighLight(language);
    ui->textEditRx->setHighLight(language);
}

void TextTRView::receiveData(const QByteArray &array)
{
    QString string, pre;
    if (ui->portReadAscii->isChecked()) {
        if (m_hexCount > 0) {
            pre = '\n';
        }
        m_hexCount = -1;
        arrayToString(string, array);
    } else {
        if (m_hexCount == -1) {
            m_hexCount = 0;
            pre = '\n';
        }
        arrayToHex(string, array, 16);
    }
    if (m_timeStamp) {
        appendTimeStamp(string);
    } else {
        ui->textEditRx->append(pre + string);
    }
}

void TextTRView::clear()
{
    ui->textEditRx->clear();
    m_asciiBuf->clear();
    m_hexCount = 0;
}

void TextTRView::setFontFamily(QString fontFamily, int size, QString style)
{
    ui->textEditRx->setFonts(fontFamily, size, Qt::black, style);
    ui->textEditTx->setFonts(fontFamily, size, Qt::black, style);
}

void TextTRView::setEnabled(bool enabled)
{
    QString str;

    ui->sendButton->setEnabled(enabled);
    str = enabled ? tr("Ctrl + Enter to send") :
        tr("Connect port and start transmission to enable this button");
    ui->sendButton->setToolTip(str);
    // auto resend
    updateResendTimerStatus();
}

// send data
void TextTRView::sendData()
{
    QByteArray array;

    if (ui->portWriteAscii->isChecked() == true) {
        QTextCodec *code = QTextCodec::codecForName(m_codecName);
        array = code->fromUnicode(ui->textEditTx->text());
    } else {
        array = QByteArray::fromHex(ui->textEditTx->text().toLatin1());
    }
    emit transmitData(array);
}

void TextTRView::onWrapBoxChanged(int status)
{
    ui->textEditRx->setWrap(status);
}

void TextTRView::onSendButtonClicked()
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

void TextTRView::updateResendTimerStatus()
{
    if (ui->sendButton->isEnabled() && ui->resendBox->isChecked()) {
        m_resendTimer->start(ui->resendIntervalBox->text().toInt());
    } else {
        m_resendTimer->stop();
    }
}

//  set resend interval time
void TextTRView::setResendInterval(int msc)
{
    m_resendTimer->setInterval(msc);
}

void TextTRView::setTextCodec(const QString &name)
{
    if (name == "GB-2312") {
        m_textCodec = GB2312;
        m_codecName = "GB-2312";
    } else if (name == "GB-18030") {
        m_textCodec = GB18030;
        m_codecName = "GB-18030";
    } else if (name == "UTF-8") {
        m_textCodec = UTF8;
        m_codecName = "UTF-8";
    } else if (name == "UTF-16BE") {
        m_textCodec = UTF16;
        m_codecName = "UTF-16BE";
    } else if (name == "UTF-16LE") {
        m_textCodec = UTF16;
        m_codecName = "UTF-16LE";
    } else { // ASCII
        m_textCodec = ASCII;
        m_codecName = "ASCII";
    }
}

void TextTRView::onHistoryBoxChanged(const QString &string)
{
    ui->textEditTx->setText(string);
}

void TextTRView::arrayToHex(QString &str, const QByteArray &array, int countOfLine)
{
    int len = array.length();
    str.resize(len * 3 + (len + m_hexCount) / countOfLine);
    for (int i = 0, j = 0; i < len; ++i) {
        uint8_t outChar = uint8_t(array[i]), t;   //每字节填充一次，直到结束
        //十六进制的转换
        t = (outChar >> 4) & 0x0F;
        str[j++] = t + (t < 10 ? '0' : 'A' - 10);
        t = outChar & 0x0F;
        str[j++] = t + (t < 10 ? '0' : 'A' - 10);
        str[j++] = ' ';
        if (m_hexCount >= countOfLine - 1) {
            m_hexCount = 0;
            str[j++] = '\n';
        } else {
            ++m_hexCount;
        }
    }
}

// array转UTF8
void TextTRView::arrayToUTF8(QString &str, const QByteArray &array)
{
    int lastIndex, cut = 0;
    bool isCut = false;

    m_asciiBuf->append(array);
    lastIndex = m_asciiBuf->length() - 1;
    if (m_asciiBuf->at(lastIndex) & 0x80) { // 0xxx xxxx -> OK
        // UTF8最大编码为4字节，因此向前搜寻三字节
        for (int i = lastIndex; i >= 0 && ++cut < 4; --i) {
            uint8_t byte = uint8_t(m_asciiBuf->at(i));
            if (((cut < 2) && (byte & 0xE0) == 0xC0) ||
                ((cut < 3) && (byte & 0xF0) == 0xE0) ||
                (byte & 0xF8) == 0xF0) {
                isCut = true;
                break;
            }
        }
    }
    lastIndex -= isCut ? cut - 1 : -1;
    QByteArray cutArray = m_asciiBuf->mid(lastIndex);
    m_asciiBuf->remove(lastIndex, cut);
    QTextCodec *code = QTextCodec::codecForName(m_codecName);
    str = code->toUnicode(*m_asciiBuf);
    m_asciiBuf->clear();
    m_asciiBuf->append(cutArray);
}

// array转双字节编码格式(GB2312等)
void TextTRView::arrayToDualByte(QString &str, const QByteArray &array)
{
    int lastIndex;
    bool isCut = false;

    m_asciiBuf->append(array);
    lastIndex = m_asciiBuf->length() - 1;
    for (int i = lastIndex; i >= 0 && m_asciiBuf->at(i) & 0x80; --i) {
        isCut = !isCut;
    }
    if (isCut) { // 字符串最末尾的非ASCII字节数不为2的整数倍
        char ch = m_asciiBuf->at(lastIndex);
        m_asciiBuf->remove(lastIndex, 1);
        QTextCodec *code = QTextCodec::codecForName(m_codecName);
        str = code->toUnicode(*m_asciiBuf);
        m_asciiBuf->clear();
        m_asciiBuf->append(ch);
    } else {
        QTextCodec *code = QTextCodec::codecForName(m_codecName);
        str = code->toUnicode(*m_asciiBuf);
        m_asciiBuf->clear();
    }
}

// array转Unicode
void TextTRView::arrayToUTF16(QString &str, const QByteArray &array)
{
    int lastIndex;
    bool isCut = false;;

    m_asciiBuf->append(array);
    lastIndex = m_asciiBuf->length() - 1;
    isCut = (lastIndex + 1) & 0x01;
    if (isCut) { // 字符串长度不为偶数
        char ch = m_asciiBuf->at(lastIndex);
        m_asciiBuf->remove(lastIndex, 1);
        QTextCodec *code = QTextCodec::codecForName(m_codecName);
        str = code->toUnicode(*m_asciiBuf);
        m_asciiBuf->clear();
        m_asciiBuf->append(ch);
    } else {
        QTextCodec *code = QTextCodec::codecForName(m_codecName);
        str = code->toUnicode(*m_asciiBuf);
        m_asciiBuf->clear();
    }
}

// array转ASCII
void TextTRView::arrayToASCII(QString &str, const QByteArray &array)
{
    if (m_asciiBuf->isEmpty()) {
        str = QString::fromLatin1(array);
    } else {
        m_asciiBuf->append(array);
        str = QString::fromLatin1(*m_asciiBuf);
        m_asciiBuf->clear();
    }
}

// 这个函数可以避免中文接收的乱码
void TextTRView::arrayToString(QString &str, const QByteArray &array)
{
    switch (m_textCodec) {
    case GB2312:
    case GB18030:
        arrayToDualByte(str, array);
        break;
    case UTF8:
        arrayToUTF8(str, array);
        break;
    case UTF16:
        arrayToUTF16(str, array);
        break;
    default: // ASCII
        arrayToASCII(str, array);
        break;
    }
}

void TextTRView::appendTimeStamp(const QString &string)
{
    QRegularExpression re(".+?(" + m_frameSeparator + "|$)");
    QRegularExpressionMatch match = re.match(string);
    QRegularExpressionMatchIterator it = re.globalMatch(string);
    QString curTime = QDateTime::currentDateTime()
            .toString(m_timeStampFormat);
    while (it.hasNext()) {
        QString span = it.next().captured(0);
        if (m_nextFrame) {
            ui->textEditRx->append(curTime);
        }
        m_nextFrame = span.indexOf(
                    QRegularExpression(m_frameSeparator + "$")) != -1;
        if (m_nextFrame) {
            span.chop(m_frameSeparator.length());
            span.append('\n');
        }
        ui->textEditRx->append(span);
    }
}

// 保存文件
void TextTRView::saveText(const QString &fname)
{
    QFile file(fname);

    if (file.open(QFile::WriteOnly)) {
        file.write(ui->textEditRx->text().toUtf8());
        file.close();
    }
}

void TextTRView::setTabsInsertSpaces(bool enable)
{
    ui->textEditTx->setIndentationsUseTabs(!enable);
}

void TextTRView::setTabWidth(int width)
{
    ui->textEditTx->setTabWidth(width);
}

void TextTRView::setAutoIndent(bool enable)
{
    ui->textEditTx->setAutoIndent(enable);
}

void TextTRView::setIndentationGuides(bool enable)
{
    ui->textEditTx->setIndentationGuides(enable);
}

QString TextTRView::saveFileFilter()
{
    return tr("Terminal Text File (*.txt)");
}

void TextTRView::saveFile(const QString &fileName, const QString &filter)
{
    if (filter == tr("Terminal Text File (*.txt)")) {
        saveText(fileName);
    }
}
