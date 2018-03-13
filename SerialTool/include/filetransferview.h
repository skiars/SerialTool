#ifndef __FILETRANSFERVIEW_H
#define __FILETRANSFERVIEW_H

#include "ui_filetransferview.h"
#include <QSettings>
#include "filethread.h"

class FileTransferView : public QWidget
{
    Q_OBJECT

public:
    FileTransferView(QWidget *parent = Q_NULLPTR);
    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);

public slots:
    void readData(const QByteArray &array);
    void cancelTransfer();
    void logOut(const QString &string, QColor color = Qt::black);

private:
    void beforceSend();

private slots:
    void browseButtonClicked();
    void portSendData(const QByteArray &array);
    void sendFile();
    void onTransFinsh();

signals:
    void sendData(const QByteArray &);

private:
    Ui_FileTransferView ui;
    FileThread thread;
    QByteArray receiveArray; // 接收缓冲区
};

#endif
