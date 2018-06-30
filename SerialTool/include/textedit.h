#ifndef __TEXTEDIT_H
#define __TEXTEDIT_H

#include <Qsci/qsciscintilla.h>

class TextEdit : public QsciScintilla
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = NULL);
    void append(const QString &text);
    void setFonts(QString fonts, int size, QColor color = Qt::black, QString style = "");
    void setHighLight(const QString &language);

public slots:
    void setWrap(bool wrap);

private:
    void setMarginsWidth();
    void highlightNone();
    void highlightCpp();
    void highlightBash();
    void highlightLua();
    void highlightJSON();

private slots:
    void onTextChanged();
    void onLinesChanged();
    void onVScrollBarRangeChanged();
    void onVScrollBarValueChanged();

private:
    QString fontFamily, m_language;
    QFont lineNumFont;
    int fontSize;
    int lineCount = 1;
    bool isWrap = false;
    bool scrollEnd = true;
};

#endif
