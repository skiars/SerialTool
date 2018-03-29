#include "xmodem.h"

// 状态机使用的字符
#define NONE 0x00 // 空闲状态
#define TRAN 0x01 // 传输状态

enum {
    None,         // 空闲状态
    Trans,        // 传输状态
    TransEnd,     // 传输结束状态
    TransEOT      // EOT字符的应答状态
};

#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define CAN 0x18

XModemClass::XModemClass(FileThread *thread)
{
    this->thread = thread;
}

void XModemClass::setThread(FileThread *thread)
{
    this->thread = thread;
}

// 计算校验值
char XModemClass::calcVerifi(const char *frame)
{
    int value = 0;

    // 只实现了校验和算法
    for (int i = 3; i < 128 + 3; ++i) {
        value += frame[i];
    }
    return (char)(value & 0xFF);
}

// 开始传输
void XModemClass::startTransmit()
{
    status = None;
    transMode = true;
}

// 开始接收
void XModemClass::startReceive()
{
    QByteArray arr;
    rxBuf.clear();
    lastCount = 0;
    transMode = false;

    arr.append(NAK);
    thread->sendPortData(arr);
}

// 取消传输
void XModemClass::cancelTrans()
{
    QByteArray arr;

    if (transMode) { // 发送模式下取消传输
        if (status == None) {
            arr.clear();
            arr.append(EOT);
            thread->sendPortData(arr);
        } else { // 接受模式下取消传输
            arr.clear();
            arr.append(CAN);
            thread->sendPortData(arr);
        }
    }
}

// 发送模式每次接收一个字节
int XModemClass::transmit(char ch, qint64 &bytes)
{
    int rBytes;
    QByteArray arr;

    if (ch == CAN) { // 取消传输
        return 1;
    }
    switch (status) {
    case None:
        if (ch == NAK) { // 开始传输
            status = ACK;
            memset(frame, 0, 132); // 初始化帧
            ch = ACK; // 为了下面传输第一帧
            status = Trans;
        }
    case TransEnd: // 传输结束
        // 接收到ACK说明最后一帧数据传输完成, 此时传输EOT
        if (ch == ACK && status == TransEnd) {
            arr.clear();
            arr.append(EOT);
            thread->sendPortData(arr);
            status = TransEOT; // 进入结束应答状态
            break;
        }
    case Trans: // 传输数据
        if (ch == ACK) { // 传输下一块数据
            frame[0] = SOH;
            ++frame[1];
            frame[2] = ~frame[1];
            rBytes = thread->readFile(frame + 3, 128);
            bytes += rBytes;
            if (rBytes < 128) { // 说明是最后一帧数据
                // 多余的字节填充0x1A
                for (int i = rBytes + 3; i < 131; ++i) {
                    frame[i] = 0x1A;
                }
                status = TransEnd;
            }
            frame[131] = calcVerifi(frame); // 计算校验字符
        }
        arr.clear();
        arr.append(frame, 132);
        thread->sendPortData(arr); // 发送数据
        break;
    case TransEOT:
        if (ch == ACK) { // EOT被对方应答, 传输结束
            status = None;
            return 1;
        }
        break;
    }
    return 0;
}

// 接收模式
int XModemClass::receive(const QByteArray &arr, qint64 &bytes)
{
    int size, pos;
    const char *pdata;
    QByteArray tbuf;

    rxBuf.append(arr);
    size = rxBuf.size();
    pdata = rxBuf.data();
    if (pdata[0] == EOT) { // 传输结束
        tbuf.append(ACK);
        thread->sendPortData(tbuf); // 发送应答标志
        return 1;
    }
    // 寻找SOH字符
    for (pos = 0; pos < size && pdata[pos] != SOH; ++pos);
    if (size - pos < 131) { // 接收区缓冲长度小于132bytes说明不是完整的帧
        return 0;
    }
    pdata += pos; // 获取帧数据
    // 校验
    if (calcVerifi(pdata) == pdata[131] // 检查校验字节
        && pdata[1] == ~pdata[2] // 帧计数校验
        && ((char)(lastCount + 1) == pdata[1] || lastCount == pdata[1])) { // 帧数是否正常
        thread->writeFile(pdata + 3, 128);
        bytes += 128;
        lastCount = pdata[1];
        tbuf.append(ACK);
        thread->sendPortData(tbuf); // 发送应答标志
    } else {
        tbuf.append(NAK);
        thread->sendPortData(tbuf); // 发送应答标志
    }
    rxBuf = rxBuf.mid(pos + 132); // 保留余下内容
    return 0;
}
