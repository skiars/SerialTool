#ifndef FILETRANSFERVIEW_H
#define FILETRANSFERVIEW_H

#include "../abstractview.h"
#include "filethread.h"

namespace Ui {
    class FileTransmitView;
}

class FileTransmitView : public AbstractView
{
    Q_OBJECT

public:
    FileTransmitView(QWidget *parent = nullptr);
    ~FileTransmitView();

    QString title() { return tr("File Transmit"); }
    QString iid() { return "file_transmit"; }
    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void setEnabled(bool enabled);

public slots:
    void receiveData(const QByteArray &array);
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

private:
    Ui::FileTransmitView *ui;
    FileThread thread;
    QByteArray receiveArray; // 接收缓冲区
};

#endif // FILETRANSFERVIEW_H
