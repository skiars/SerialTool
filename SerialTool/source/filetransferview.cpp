#include "filetransferview.h"
#include "ui_filetransferview.h"
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore>
#include "xmodem.h"

// 构造函数
FileTransferView::FileTransferView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTransferView)
{
    ui->setupUi(this);

    connect(&thread, &FileThread::sendData, this, &FileTransferView::portSendData);
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(cancelTransfer()));
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(sendFile()));
    connect(&thread, SIGNAL(transFinsh()), this, SLOT(onTransFinsh()));
    connect(&thread, SIGNAL(timeout()), this, SLOT(onTimeoutError()));
}

FileTransferView::~FileTransferView()
{
    delete ui;
}

void FileTransferView::retranslate()
{
    ui->retranslateUi(this);
}

/* 读取配置文件 */
void FileTransferView::loadConfig(QSettings *config)
{
    config->beginGroup("FileTransmit");
    ui->beforeSendEdit->setText(config->value("BeforeSendText").toString());
    ui->enableBerforSendBox->setChecked(config->value("BeforeSend").toBool());
    ui->pathBox->setText(config->value("FileName").toString());
    ui->protocolBox->setCurrentText(config->value("Protocol").toString());
    if (config->value("SendMode").toBool()) {
        ui->sendButton->setChecked(true);
    } else {
        ui->receiveButton->setChecked(true);
    }
    config->endGroup();
}

// 保存配置
void FileTransferView::saveConfig(QSettings *config)
{
    config->beginGroup("FileTransmit");
    config->setValue("BeforeSendText", QVariant(ui->beforeSendEdit->toPlainText()));
    config->setValue("BeforeSend", QVariant(ui->enableBerforSendBox->isChecked()));
    config->setValue("FileName", QVariant(ui->pathBox->text()));
    config->setValue("Protocol", QVariant(ui->protocolBox->currentText()));
    config->setValue("SendMode", QVariant(ui->sendButton->isChecked()));
    config->endGroup();
}

// 打开文件按钮按下
void FileTransferView::browseButtonClicked()
{
    QString fname;
    if (ui->sendButton->isChecked()) {
        fname = QFileDialog::getOpenFileName(this, "Open File", ui->pathBox->text());
    } else {
        fname = QFileDialog::getSaveFileName(this, "Open File", ui->pathBox->text());
    }
    if (!fname.isEmpty()) { // 文件名有效
        ui->pathBox->setText(fname);
    }
}

// 发送文件
void FileTransferView::sendFile()
{
    bool res = true;
    FileThread::TransMode mode[] {
        FileThread::SendMode,
        FileThread::ReceiveMode
    };
    QFile file(ui->pathBox->text());

    if (ui->sendButton->isChecked()) { // 发送模式要求文件可以以只读方式打开
        res = file.open(QFile::ReadOnly);
        if (res) {
            file.close();
        }
    } else { // 接收模式要求文件可以以只写方式打开
        res = file.open(QFile::WriteOnly);
        if (res) {
            file.close();
        }
    }
    if (res) {
        thread.setFileName(ui->pathBox->text());
        thread.setProtocol(FileThread::XModem);
        thread.setTransMode(mode[!ui->sendButton->isChecked()]);
        beforceSend(); // 预发送文本
        thread.startTransmit();
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        QString string(tr("Start transmit file: \"")
            + ui->pathBox->text() + "\".");
        logOut(string, Qt::blue);
    } else { // 无法打开文件
        QString string(tr("Can not open the file: \"")
            + ui->pathBox->text() + "\".\n");

        logOut(tr("Error: ") + string, Qt::red);
        QMessageBox err(QMessageBox::Critical,
            tr("Error"),
            string,
            QMessageBox::Cancel, this);
        err.exec();
    }
}

// 发送数据
void FileTransferView::portSendData(const QByteArray &array)
{
    QString string;
    emit sendData(array);

    ui->progressBar->setValue(thread.progress());
}

// 接收数据
void FileTransferView::readData(const QByteArray &array)
{
    thread.readData(array);
}

// 取消发送
void FileTransferView::cancelTransfer()
{
    thread.cancelTransmit();
    logOut(tr("Cancel transfer.\n"), Qt::darkGray);
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

// 显示一条log
void FileTransferView::logOut(const QString &string, QColor color)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");

    ui->textEdit->setTextColor(color);
    ui->textEdit->append("[" + time + "] " + string);
}

// 传输完成槽
void FileTransferView::onTransFinsh()
{
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    logOut(tr("Transmit finished.\n"), Qt::darkGreen);
}

// 预发送文本
void FileTransferView::beforceSend()
{
    // 只有在发送模式下才可以使用预发送模式
    if (ui->sendButton->isChecked() && ui->enableBerforSendBox->isChecked()) {
        QTextCodec *code = QTextCodec::codecForName("GB-2312");
        QByteArray arr = code->fromUnicode(ui->beforeSendEdit->toPlainText());

        emit sendData(arr);
    }
}

void FileTransferView::onTimeoutError()
{
    QString string(tr("Transmission timeout."));

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    logOut(tr("Error: ") + string, Qt::red);
    QMessageBox err(QMessageBox::Critical,
        tr("Error"),
        string,
        QMessageBox::Cancel, this);
    err.exec();
}
