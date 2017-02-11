#include "textedit.h"
#include <QScrollBar>

TextEdit::TextEdit(QWidget *parent) : QsciScintilla(parent)
{
    SendScintilla(SCI_SETCODEPAGE, SC_CP_UTF8);
    SendScintilla(SCI_SETLAYOUTCACHE, SC_CACHE_DOCUMENT);
    connect(this, &QsciScintilla::textChanged, this, &TextEdit::onTextChanged);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &TextEdit::onVScrollBarRangeChanged);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::onVScrollBarValueChanged);
    connect(this, &QsciScintilla::linesChanged, this, &TextEdit::onLinesChanged);
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
    // 如果光标之前就在末尾时将光标移动到最后
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

void TextEdit::onTextChanged()
{
    if (text().isEmpty()) {
        SendScintilla(SCI_SETSCROLLWIDTH, 1);
    }
    // 如果使用了自动换行功能并且滚动条在底部则把滚动条重新移动到底部
    if (scrollEnd && isWrap) {
        int lineEnd = lineCount - 1; // 末行
        int lineLen = SendScintilla(SCI_LINELENGTH, lineEnd);
        SendScintilla(SCI_LINESCROLL, lineLen, lineEnd);
    }
}

// 计算一个十进制数字的位数
static int countOrder(int value)
{
    int order = 0;
    for (int i = 1; i <= value; i *= 10) {
        ++order;
    }
    return order;
}

void TextEdit::onLinesChanged()
{
    int temp;
    static int order = 0;

    lineCount = SendScintilla(SCI_GETLINECOUNT);
    temp = countOrder(lineCount);
    if (temp != order) {
        order = temp;
        setMarginsWidth();
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
