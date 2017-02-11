#include "textedit.h"
#include <QScrollBar>

TextEdit::TextEdit(QWidget *parent) : QsciScintilla(parent)
{
    SendScintilla(SCI_SETCODEPAGE, SC_CP_UTF8);
    SendScintilla(SCI_SETLAYOUTCACHE, SC_CACHE_DOCUMENT);
    connect(this, &QsciScintilla::textChanged, this, &TextEdit::setHScrollBarWidth);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &TextEdit::onVScrollBarRangeChanged);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::onVScrollBarValueChanged);
    //connect(horizontalScrollBar(), &QScrollBar::rangeChanged, this, &TextEdit::onHScrollBarRangeChanged);
    //connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::onHScrollBarValueChanged);
}

void TextEdit::setText(const QString &text)
{
    clear();
    append(text);
}

void TextEdit::append(const QString &text)
{
    bool posEnd = SendScintilla(SCI_GETLENGTH) == SendScintilla(SCI_GETCURRENTPOS);
    QsciScintilla::append(text);
    // 设置页边宽度
    static int order = 1;
    lineCount = SendScintilla(QsciScintillaBase::SCI_GETLINECOUNT);
    if (lineCount >= order * 10) {
        order *= 10;
        setMarginsWidth();
    }
    // 将光标移动到最后
    if (posEnd) {
        long pos = SendScintilla(SCI_GETLENGTH);
        SendScintilla(SCI_SETANCHOR, pos);
        SendScintilla(SCI_SETCURRENTPOS, pos);
    }
}

void TextEdit::setFonts(QString fonts, int size, QColor color, QString style)
{
    fontFamily = fonts;
    fontSize = size;
    lineNumFont = QFont(fonts.section('+', 0, 0), size);
    // 文本显示
    SendScintilla(SCI_STYLESETFONT, fonts.toStdString().c_str());
    SendScintilla(SCI_STYLESETSIZE, 0, size);
    SendScintilla(SCI_STYLESETFORE, 0, color);
    if (style.indexOf("bold") != -1) { // 粗体字
        SendScintilla(SCI_STYLESETBOLD, (int)0, true);
    } else {
        SendScintilla(SCI_STYLESETBOLD, (int)0, (long)false);
    }
    if (style.indexOf("italic") != -1) { // 斜体字
        SendScintilla(SCI_STYLESETITALIC, (int)0, true);
    } else {
        SendScintilla(SCI_STYLESETITALIC, (int)0, (long)false);
    }

    // 光标颜色
    SendScintilla(SCI_SETCARETFORE, color);

    // 页边显示行号
    SendScintilla(SCI_STYLESETFONT, STYLE_LINENUMBER,
        fonts.section('+', 0, 0).toStdString().c_str());
    SendScintilla(SCI_STYLESETSIZE, STYLE_LINENUMBER, size);
    SendScintilla(SCI_STYLESETFORE, STYLE_LINENUMBER, 0xFF4030);
    SendScintilla(SCI_SETMARGINTYPEN, 1, SC_MARGIN_NUMBER);
    setMarginsWidth();
    
    SendScintilla(SCI_SETSCROLLWIDTH, 1);
    /* If scroll width tracking is enabled then the scroll width is adjusted
       to ensure that all of the lines currently displayed can be completely scrolled.
       This mode never adjusts the scroll width to be narrower.
       */
    SendScintilla(SCI_SETSCROLLWIDTHTRACKING, true);
}

void TextEdit::setMarginsWidth()
{
    QString str = QString::number(lineCount);
    QFontMetrics fm(lineNumFont);
    int len = fm.boundingRect(str + "  ").width();
    SendScintilla(SCI_SETMARGINWIDTHN, 1, len);
}

void TextEdit::setHScrollBarWidth()
{
    if (text().isEmpty()) {
        SendScintilla(SCI_SETSCROLLWIDTH, 1);
    }
    if (scrollEnd && isWrap) {
        SendScintilla(SCI_GOTOPOS, SendScintilla(SCI_GETLENGTH));
    }
}

void TextEdit::onVScrollBarRangeChanged()
{
    if (scrollEnd) {
        QScrollBar *scrollBar = verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

void TextEdit::onVScrollBarValueChanged()
{
    QScrollBar *scrollBar = verticalScrollBar();
    if (scrollBar->value() != scrollBar->maximum()) {
        scrollEnd = false;
    } else {
        scrollEnd = true;
    }
}

void TextEdit::setWrap(bool wrap)
{
    if (wrap) {
        QsciScintilla::setWrapMode(WrapWhitespace);
    } else {
        QsciScintilla::setWrapMode(WrapNone);
    }
    isWrap = wrap;
}
