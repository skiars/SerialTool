#include "filethread.h"
#include "xmodem.h"
#include <QMutexLocker>
#include <QDebug>

static XModemClass xmodem;

FileThread::FileThread()
{
    xmodem.setThread(this);
    transMode = StopMode;
    start();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));

    m_timer.setInterval(500);
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
void FileThread::startTransmit()
{
    setStatus(StartTrans);
}

// 取消传输
void FileThread::cancelTransmit()
{
    setStatus(CancelTrans);
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
    m_mutex.lock();
    m_rxbuffer = array;
    m_mutex.unlock();
    setStatus(ReadData);
}

void FileThread::setStatus(Status status)
{
    m_mutex.lock();
    m_status = status;
    m_mutex.unlock();
    m_sem.release();
}

// 发送数据
void FileThread::sendPortData(const QByteArray &array)
{
    emit sendData(array);
}

// 线程函数
void FileThread::run()
{
    forever {
        m_sem.acquire();
        m_mutex.lock();
        Status status = m_status;
        QByteArray array = m_rxbuffer;
        m_status = None;
        m_mutex.unlock();

        QTimer::singleShot(0, &m_timer,SLOT(stop()));

        switch (status) {
        case ReadData:
            receivePack_p(array);
            break;
        case StartTrans:
            m_timeoutCount = 0;
            startTransmit_p();
            break;
        case CancelTrans:
            cancelTransmit_p();
            break;
        default:
            break;
        }
    }
}

void FileThread::writeFile(const char* buffer, int size)
{
    m_file->write(buffer, size);
}

int FileThread::readFile(char* buffer, int size)
{
    return m_file->read(buffer, size);
}

void FileThread::receivePack_p(const QByteArray &array)
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
        closeFile();
        transMode = StopMode;
        emit transFinsh();
    }
}

void FileThread::startTransmit_p()
{
    bool res;
    m_file = new QFile(fileName);
    if (transMode == SendMode) {
        res = m_file->open(QFile::ReadOnly);
        if (res) {
            switch (protocol) {
            case XModem:
                xmodem.startTransmit();
                break;
            default:
                break;
            }
        }
    } else {
        res = m_file->open(QFile::WriteOnly);
        if (res) {
            switch (protocol) {
            case XModem:
                xmodem.startReceive();
                QTimer::singleShot(0, &m_timer,SLOT(start()));
                break;
            default:
                break;
            }
        }
    }
    if (res) {
        fSize = m_file->size();
        fPos = 0;
    } else {
        emit fileError(); // connot open file
    }
}

// 取消传输（私有）
void FileThread::cancelTransmit_p()
{
    switch (protocol) {
    case XModem:
        xmodem.cancelTrans();
        break;
    default:
        break;
    }
    closeFile();
    transMode = StopMode;
}

void FileThread::onTimerUpdate()
{
    if (m_timeoutCount++ >= 10) {
        closeFile();
        m_timer.stop();
        emit timeout();
    } else {
        startTransmit_p();
    }
}

void FileThread::closeFile() {
    m_file->close();
    delete m_file;
    m_file = nullptr;
}
