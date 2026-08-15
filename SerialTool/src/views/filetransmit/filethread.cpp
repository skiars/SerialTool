#include "filethread.h"
#include "xmodem.h"
#include <QMutexLocker>
#include <QDebug>

static XModemClass xmodem;

FileThread::FileThread() :
    protocol(XModem),
    transMode(StopMode),
    fSize(0),
    fPos(0),
    m_file(nullptr),
    m_status(None),
    m_timeoutCount(0),
    m_stopping(false)
{
    xmodem.setThread(this);
    start();
}

FileThread::~FileThread()
{
    stopThread();
    wait();
}

// 发送文件
void FileThread::setFileName(const QString &fileName)
{
    m_mutex.lock();
    this->fileName = fileName;
    m_mutex.unlock();
}

// 设置传输协议
void FileThread::setProtocol(Protocol mode)
{
    m_mutex.lock();
    protocol = mode;
    m_mutex.unlock();
}

// 设置收发模式
void FileThread::setTransMode(TransMode mode)
{
    m_mutex.lock();
    transMode = mode;
    m_mutex.unlock();
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
    QMutexLocker lock(&m_mutex);
    return fSize;
}

// 获取文件偏移
qint64 FileThread::filePos()
{
    QMutexLocker lock(&m_mutex);
    return fPos;
}

// 获取传输进度
char FileThread::progress()
{
    QMutexLocker lock(&m_mutex);
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
    m_status = ReadData;
    m_mutex.unlock();
    m_cond.wakeOne();
}

// 请求线程退出
void FileThread::stopThread()
{
    m_mutex.lock();
    m_stopping = true;
    m_mutex.unlock();
    m_cond.wakeAll();
}

void FileThread::setStatus(Status status)
{
    m_mutex.lock();
    m_status = status;
    m_mutex.unlock();
    m_cond.wakeOne();
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
        m_mutex.lock();
        while (m_status == None && !m_stopping) {
            // 接收模式等待接收数据, 无数据时定时重发 NAK 请求
            bool active = (transMode == ReceiveMode && m_file != nullptr);
            bool woke = active ? m_cond.wait(&m_mutex, 500) : m_cond.wait(&m_mutex);
            if (!woke && !m_stopping) {
                m_mutex.unlock();
                handleTimeout();
                m_mutex.lock();
            }
        }
        if (m_stopping) {
            m_mutex.unlock();
            break;
        }
        Status status = m_status;
        QByteArray array = m_rxbuffer;
        m_status = None;
        m_mutex.unlock();

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

// 接收模式空闲超时处理, 在工作线程执行
void FileThread::handleTimeout()
{
    if (transMode == ReceiveMode && m_file != nullptr) {
        if (++m_timeoutCount > 10) {
            closeFile();
            transMode = StopMode;
            emit timeout();
        } else {
            xmodem.sendNak();
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
    QMutexLocker lock(&m_mutex);
    qint64 pos = fPos;

    if (transMode == SendMode) { // 发送模式
        switch (protocol) {
        case XModem:
            if (!array.isEmpty()) {
                result = xmodem.transmit(array[0], pos);
            }
            break;
        default:
            break;
        }
    } else if (transMode == ReceiveMode) { // 接收模式
        switch (protocol) {
        case XModem:
            result = xmodem.receive(array, pos);
            break;
        default:
            break;
        }
        m_timeoutCount = 0; // 收到数据则重置超时计数
    }
    fPos = pos;
    fSize = fPos;
    lock.unlock();
    if (result) { // 传输结束
        closeFile();
        transMode = StopMode;
        emit transFinsh();
    }
}

void FileThread::startTransmit_p()
{
    bool res;
    closeFile();
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
        closeFile();
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

void FileThread::closeFile() {
    if (m_file != nullptr) {
        m_file->close();
        delete m_file;
        m_file = nullptr;
    }
}