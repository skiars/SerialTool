#ifndef __FILETHREAD_H
#define __FILETHREAD_H

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

public :
    FileThread();
    void setFileName(const QString &fileName);
    void setProtocol(Protocol mode);
    void setTransMode(TransMode mode);
    bool startTransfer();
    bool cancelTransfer();
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

protected:
    void run();

private:
    Protocol protocol;
    TransMode transMode;
    qint64 fSize, fPos;
    QByteArray receiveArray; // Ω” ’ª∫≥Â«¯
    QString fileName;
    QFile *file;
};

#endif
