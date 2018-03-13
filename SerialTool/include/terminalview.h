#ifndef TERMINALVIEW_H
#define TERMINALVIEW_H

#include <QWidget>

namespace Ui {
class TerminalView;
}

class QTimer;
class QSettings;
class QByteArray;

class TerminalView : public QWidget
{
    Q_OBJECT

public:
    explicit TerminalView(QWidget *parent = 0);
    ~TerminalView();

    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void append(const QByteArray &array);
    void clear();
    void setFontFamily(QString fonts, int size, QString style);
    void setEnabled(bool status);
    void setPaused(bool status);
    void saveText(const QString &fname);

signals:
    void sendDataRequest(const QByteArray &array);

private:
    void keyPressEvent(QKeyEvent  *event);
    void setSendButtonEnabled(bool status);
    void arrayToHex(QString &str, const QByteArray &arr, int countOfLine);
    void arrayToAscii(QString &str, const QByteArray &arr);
    void loadHistory(QSettings *config);
    void saveHistory(QSettings *config);

private slots:
    void sendData();
    void onWrapBoxChanged(int status);
    void onSendButtonClicked();
    void updateResendTimerStatus();
    void setResendInterval(int msc);
    void onHistoryBoxChanged(const QString &string);

private:
    Ui::TerminalView *ui;
    bool sendEnabled = false,  paused = false;
    QTimer *resendTimer;
    QByteArray *asciiBuf;
};

#endif // TERMINALVIEW_H
