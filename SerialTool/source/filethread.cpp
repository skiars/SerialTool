#include "filethread.h"
#include "xmodem.h"

static XModemClass xmodem;

FileThread::FileThread()
{
    xmodem.setThread(this);
    transMode = StopMode;
}

// 发送文件
void FileThread::setFileName(const QString &fileName)
{
    this->fileName = fileName;
}

// 设置传输协议
void FileThread::setProtocol(Protocol mode)
{
    protocol = mode;
}

// 设置收发模式
void FileThread::setTransMode(TransMode mode)
{
    transMode = mode;
}

// 启动传输
bool FileThread::startTransfer()
{
    bool res;
    file = new QFile(fileName);
    if (transMode == SendMode) {
        res = file->open(QFile::ReadOnly);
        switch (protocol) {
        case XModem:
            xmodem.startTransmit();
            break;
        default:
            break;
        }
    } else {
        res = file->open(QFile::WriteOnly);
        switch (protocol) {
        case XModem:
            xmodem.startReceive();
            break;
        default:
            break;
        }
    }
    fSize = file->size();
    fPos = 0;
    return res;
}

// 取消传输
bool FileThread::cancelTransfer()
{
    bool res = true;

    switch (protocol) {
    case XModem:
        res = xmodem.cancelTrans();
        break;
    default:
        break;
    }
    if (res) {
        file->close();
        transMode = StopMode;
    }
    return res;
}

// 获取文件大小
qint64 FileThread::fileSize()
{
    return fSize;
}

// 获取文件偏移
qint64 FileThread::filePos()
{
    return fPos;
}

// 获取传输进度
char FileThread::progress()
{
    if (fSize == 0) {
        return 0;
    }
    return (char)(fPos * 100 / fSize);
}

// 读取数据槽函数
void FileThread::readData(const QByteArray &array)
{
    char result = 0;

    if (transMode == SendMode) { // 发送模式
        switch (protocol) {
        case XModem:
            result = xmodem.transmit(array[0], fPos);
            break;
        default:
            break;
        }
    } else if (transMode == ReceiveMode) { // 接收模式
        switch (protocol) {
        case XModem:
            result = xmodem.receive(array, fPos);
            break;
        default:
            break;
        }
        fSize = fPos;
    }
    if (result) { // 传输结束
        file->close();
        delete file;
        file = nullptr;
        transMode = StopMode;
        emit transFinsh();
    }
}

// 发送数据
void FileThread::sendPortData(const QByteArray &array)
{
    emit sendData(array);
}

// 线程函数
void FileThread::run()
{
    exec();
}

void FileThread::writeFile(const char* buffer, int size)
{
    file->write(buffer, size);
}

int FileThread::readFile(char* buffer, int size)
{
    return file->read(buffer, size);
}
