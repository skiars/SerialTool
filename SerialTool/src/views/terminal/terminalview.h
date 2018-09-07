#ifndef TERMINALVIEW_H
#define TERMINALVIEW_H

#include "../abstractview.h"

namespace Ui {
class TerminalView;
}

class TerminalView : public AbstractView
{
    Q_OBJECT

public:
    explicit TerminalView(QWidget *parent = nullptr);
    ~TerminalView();

    QString title() { return tr("Terminal"); }
    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void loadSettings(QSettings *config);
    void receiveData(const QByteArray &array);
    void setEnabled(bool enabled);
    void clear();

    //QString openFileFilter();
    QString saveFileFilter();
    void saveFile(const QString &fileName, const QString &filter);

private:
    void setFontFamily(QString fonts, int size, QString style);
    void setHighlight(const QString &language);
    void setTextCodec(const QString &name);
    void setTabsInsertSpaces(bool enable);
    void setTabWidth(int width);
    void setAutoIndent(bool enable);
    void setIndentationGuides(bool enable);
    void saveText(const QString &fname);

    void keyPressEvent(QKeyEvent  *event);
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
    QTimer *m_resendTimer;
    QByteArray *m_asciiBuf;
    enum TextCodec m_textCodec;
    QByteArray m_codecName;
};

#endif // TERMINALVIEW_H
