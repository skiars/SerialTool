#ifndef __FILETHREAD_H
#define __FILETHREAD_H

#include <QThread>
#include <QColor>
#include <QFile>
#include <QMutex>
#include <QSemaphore>
#include <QWaitCondition>
#include <QTimer>

class FileThread : public QThread
{
    Q_OBJECT

public:
    enum Protocol {
        XModem,
        YModem,
        ZModem
    };

    enum TransMode {
        StopMode,
        SendMode,
        ReceiveMode
    };

public :
    FileThread();
    void setFileName(const QString &fileName);
    void setProtocol(Protocol mode);
    void setTransMode(TransMode mode);
    void startTransmit();
    void cancelTransmit();
    qint64 fileSize();
    qint64 filePos();
    char progress();
    void writeFile(const char* buffer, int size);
    int readFile(char* buffer, int size);
    void sendPortData(const QByteArray &array);

public slots:
    void readData(const QByteArray &array);

signals:
    void sendData(const QByteArray &);
    void transFinsh(void);
    void fileError();
    void timeout();

protected:
    void run();

private slots:
    void onTimerUpdate();

private:
    enum Status {
        None = 0,
        StartTrans,
        CancelTrans,
        ReadData
    };

    void setStatus(Status status);
    void startTransmit_p();
    void receivePack_p(const QByteArray &array);
    void cancelTransmit_p();
    void closeFile();

private:
    Protocol protocol;
    TransMode transMode;
    qint64 fSize, fPos;
    QMutex m_mutex;
    QSemaphore m_sem;
    QByteArray receiveArray; // 接收缓冲区
    QString fileName;
    QFile *m_file;
    Status m_status = None;
    QByteArray m_rxbuffer;
    QTimer m_timer;
    int m_timeoutCount;
};

#endif
