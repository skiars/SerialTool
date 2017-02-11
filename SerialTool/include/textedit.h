#ifndef __TEXTEDIT_H
#define __TEXTEDIT_H

#include <Qsci/qsciscintilla.h>

class TextEdit : public QsciScintilla
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = NULL);
    void setText(const QString &text);
    void append(const QString &text);
    void setFonts(QString fonts, int size, QColor color = Qt::black, QString style = "");

public slots:
    void setWrap(bool wrap);

private:
    void setMarginsWidth();

private slots:
    void setHScrollBarWidth();
    void onVScrollBarRangeChanged();
    void onVScrollBarValueChanged();
    void onHScrollBarRangeChanged();
    void onHScrollBarValueChanged();

private:
    QString fontFamily;
    QFont lineNumFont;
    int fontSize;
    int lineCount = 1;
    bool hScrollEnd = true;
    bool vScrollEnd = true;
};

#endif
