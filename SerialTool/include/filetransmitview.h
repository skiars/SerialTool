#ifndef __FILETRANSFERVIEW_H
#define __FILETRANSFERVIEW_H

#include <QWidget>
#include "filethread.h"

namespace Ui {
    class FileTransmitView;
}
class QSettings;

class FileTransmitView : public QWidget
{
    Q_OBJECT

public:
    FileTransmitView(QWidget *parent = Q_NULLPTR);
    ~FileTransmitView();
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
    void onTimeoutError();

signals:
    void sendData(const QByteArray &);

private:
    Ui::FileTransmitView *ui;
    FileThread thread;
    QByteArray receiveArray; // 接收缓冲区
};

#endif
