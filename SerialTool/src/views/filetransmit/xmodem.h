#ifndef XMODEM_H
#define XMODEM_H

#include <QByteArray>

#define XMODEM_SOH 0x01
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18

class FileThread;

class XModemClass {
public :
    XModemClass(FileThread *thread = nullptr);
    int transmit(char ch, qint64 &bytes);
    int receive(const QByteArray &arr, qint64 &bytes);
    void startTransmit();
    void startReceive();
    void sendNak();
    void cancelTrans();

private:
    char calcVerifi(const char *frame);

private:
    bool transMode;
    char status;
    char lastCount;
    char frame[132];    // 数据帧
    FileThread *thread;
    QByteArray rxBuf;   // 接收缓冲
};

#endif // XMODEM_H
