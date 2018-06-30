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
    void setHighlight(const QString &language);
    void setTextCodec(const QString &name);
    void saveText(const QString &fname);

signals:
    void sendDataRequest(const QByteArray &array);

private:
    void keyPressEvent(QKeyEvent  *event);
    void setSendButtonEnabled(bool status);
    void arrayToHex(QString &str, const QByteArray &arr, int countOfLine);
    void arrayToString(QString &str, const QByteArray &arr);
    void loadHistory(QSettings *config);
    void saveHistory(QSettings *config);

    void arrayToUTF8(QString &str, const QByteArray &array);
    void arrayToUTF16(QString &str, const QByteArray &array);
    void arrayToDualByte(QString &str, const QByteArray &array);
    void arrayToASCII(QString &str, const QByteArray &array);

private slots:
    void sendData();
    void onWrapBoxChanged(int status);
    void onSendButtonClicked();
    void updateResendTimerStatus();
    void setResendInterval(int msc);
    void onHistoryBoxChanged(const QString &string);

private:
    enum TextCodec {
        ASCII,
        GB2312,
        GB18030,
        UTF8,
        UTF16
    };

    Ui::TerminalView *ui;
    bool m_sendEnabled = false,  m_paused = false;
    QTimer *m_resendTimer;
    QByteArray *m_asciiBuf;
    enum TextCodec m_textCodec;
    QByteArray m_codecName;
};

#endif // TERMINALVIEW_H
