#ifndef TEXTTRVIEW_H
#define TEXTTRVIEW_H

#include "../abstractview.h"

namespace Ui {
class TextTRView;
}

class TextTRView : public AbstractView
{
    Q_OBJECT

public:
    explicit TextTRView(QWidget *parent = nullptr);
    ~TextTRView();

    QString title() { return tr("Text Tx/Rx"); }
    QString iid() { return "text_tx/rx"; }
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

    void appendTimeStamp(const QString &string);

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

    Ui::TextTRView *ui;
    int m_hexCount = 0;
    QTimer *m_resendTimer;
    QByteArray *m_asciiBuf;
    enum TextCodec m_textCodec;
    QByteArray m_codecName;
    bool m_nextFrame = true, m_timeStamp = false;
    QString m_timeStampFormat, m_frameSeparator;
};

#endif // TEXTTRVIEW_H
