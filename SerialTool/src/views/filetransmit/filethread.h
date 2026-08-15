#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QThread>
#include <QColor>
#include <QFile>
#include <QMutex>
#include <QWaitCondition>

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

    enum Status {
        None = 0,
        StartTrans,
        CancelTrans,
        ReadData
    };

public :
    FileThread();
    ~FileThread();
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
    void stopThread();

signals:
    void sendData(const QByteArray &);
    void transFinsh(void);
    void fileError();
    void timeout();

protected:
    void run();

private:
    void setStatus(Status status);
    void handleTimeout();
    void startTransmit_p();
    void receivePack_p(const QByteArray &array);
    void cancelTransmit_p();
    void closeFile();

private:
    Protocol protocol;
    TransMode transMode;
    qint64 fSize, fPos;
    QMutex m_mutex;
    QWaitCondition m_cond;
    QByteArray receiveArray; // 接收缓冲区
    QString fileName;
    QFile *m_file;
    Status m_status;
    QByteArray m_rxbuffer;
    int m_timeoutCount;
    bool m_stopping;
};

#endif // FILETHREAD_H