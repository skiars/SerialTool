#include "textedit.h"
#include <QtCore>
#include <QScrollBar>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexerjson.h>

TextEdit::TextEdit(QWidget *parent) : QsciScintilla(parent)
{
    SendScintilla(SCI_SETCODEPAGE, SC_CP_UTF8);
    SendScintilla(SCI_SETLAYOUTCACHE, SC_CACHE_DOCUMENT);

    setTabIndents(true); // indent alignment
    setAutoIndent(true);
    setTabWidth(4);
    setIndentationsUseTabs(false);
    setIndentationGuides(false);

    connect(this, &QsciScintilla::textChanged, this, &TextEdit::onTextChanged);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, &TextEdit::onVScrollBarRangeChanged);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::onVScrollBarValueChanged);
    connect(this, &QsciScintilla::linesChanged, this, &TextEdit::onLinesChanged);
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
    lineNumFont = QFont(fonts.section(',', 0, 0), size);
    // 文本显示
    SendScintilla(SCI_STYLESETFONT, STYLE_DEFAULT, fonts.toStdString().c_str());
    SendScintilla(SCI_STYLESETSIZE, STYLE_DEFAULT, size);
    SendScintilla(SCI_STYLESETFORE, STYLE_DEFAULT, color);
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
    SendScintilla(SCI_STYLECLEARALL); // 设置全局风格

    // 光标颜色
    SendScintilla(SCI_SETCARETFORE, color);

    // 页边显示行号
    SendScintilla(SCI_STYLESETFONT, STYLE_LINENUMBER,
        fonts.section(',', 0, 0).toStdString().c_str());
    SendScintilla(SCI_STYLESETSIZE, STYLE_LINENUMBER, size);
    SendScintilla(SCI_STYLESETFORE, STYLE_LINENUMBER, 0xFF8050);
    SendScintilla(SCI_SETMARGINTYPEN, 1, SC_MARGIN_NUMBER);
    setMarginsWidth();
    
    SendScintilla(SCI_SETSCROLLWIDTH, 1);
    /* If scroll width tracking is enabled then the scroll width is adjusted
       to ensure that all of the lines currently displayed can be completely scrolled.
       This mode never adjusts the scroll width to be narrower.
       */
    SendScintilla(SCI_SETSCROLLWIDTHTRACKING, true);

    setHighLight(m_language); // 重新设置语法高亮
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
        setWrapMode(WrapWhitespace);
    } else {
        setWrapMode(WrapNone);
    }
    isWrap = wrap;
}

void TextEdit::highlightNone()
{
    SendScintilla(SCI_SETLEXER, SCLEX_CONTAINER);
}

void TextEdit::highlightCpp()
{
    const char* g_szKeywords =
        "asm auto bool break case catch char class const "
        "const_cast continue default delete do double "
        "dynamic_cast else enum explicit extern false finally "
        "float for friend goto if inline int long mutable "
        "namespace new operator private protected public "
        "register reinterpret_cast register return short signed "
        "sizeof static static_cast struct switch template "
        "this throw true try typedef typeid typename "
        "union unsigned using virtual void volatile "
        "wchar_t while";

    SendScintilla(SCI_SETLEXER, SCLEX_CPP); // C++解析器
    SendScintilla(SCI_SETKEYWORDS, (unsigned long)0, g_szKeywords);// 设置关键字

    // 下面设置各种语法元素前景色
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::Default, 0x38312A);   // 默认
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::Keyword, 0x8B8B00);   // 关键字
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::DoubleQuotedString, 0x6666D4); // 字符串
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::SingleQuotedString, 0x6666D4); // 字符
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::Operator, 0xB48246); // 运算符
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::Number, 0x006F7F); // 数字
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::PreProcessor, 0x808080); // 预处理指令
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::Comment, 0x008000); // 块注释
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::CommentLine, 0x008000); // 行注释
    SendScintilla(SCI_STYLESETFORE, QsciLexerCPP::CommentDoc, 0x008000); // 文档注释（/**开头）
}

void TextEdit::highlightBash()
{
    QByteArray keyWords;

    QFile file(QStandardPaths::writableLocation(
        QStandardPaths::AppConfigLocation) + "/keywords");

    if (file.open(QIODevice::ReadOnly)) {
        keyWords = file.readAll();
        file.close();
    }

    SendScintilla(SCI_SETLEXER, SCLEX_BASH); // bash解析器
    SendScintilla(SCI_SETKEYWORDS, (unsigned long)0, keyWords.data());// 设置关键字
    // 下面设置各种语法元素前景色
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::Default, 0x38312A); // 默认
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::Keyword, 0x8B8B00);   // 关键字
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::DoubleQuotedString, 0x6666D4); // 字符串
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::SingleQuotedString, 0x6666D4); // 字符
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::Operator, 0xB48246); // 运算符
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::Number, 0x006F7F); // 数字
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::Comment, 0x008000); // 行注释
    SendScintilla(SCI_STYLESETFORE, QsciLexerBash::Identifier, 0x38312A); // 识别符
}

void TextEdit::highlightLua()
{
    const char* g_szKeywords =
        "and break do else elseif end false for function "
        "if in local nil not or repeat return then true until while";

    SendScintilla(SCI_SETLEXER, SCLEX_LUA); // lua解析器
    SendScintilla(SCI_SETKEYWORDS, (unsigned long)0, g_szKeywords);// 设置关键字
    // 下面设置各种语法元素前景色
    SendScintilla(SCI_STYLESETFORE, QsciLexerLua::Default, 0x38312A); // 默认
    SendScintilla(SCI_STYLESETFORE, QsciLexerLua::Keyword, 0x8B8B00);   // 关键字
    SendScintilla(SCI_STYLESETFORE, QsciLexerLua::String, 0x6666D4); // 字符串
    SendScintilla(SCI_STYLESETFORE, QsciLexerLua::Operator, 0xB48246); // 运算符
    SendScintilla(SCI_STYLESETFORE, QsciLexerLua::Number, 0x006F7F); // 数字
    SendScintilla(SCI_STYLESETFORE, QsciLexerLua::Comment, 0x008000); // 行注释
    SendScintilla(SCI_STYLESETFORE, QsciLexerLua::Identifier, 0x38312A); // 识别符
}

void TextEdit::highlightJSON()
{
    const char* g_szKeywords = "true false";

    SendScintilla(SCI_SETLEXER, SCLEX_JSON); // lua解析器
    SendScintilla(SCI_SETKEYWORDS, (unsigned long)0, g_szKeywords);// 设置关键字
    // 下面设置各种语法元素前景色
    SendScintilla(SCI_STYLESETFORE, QsciLexerJSON::Default, 0x38312A); // 默认
    SendScintilla(SCI_STYLESETFORE, QsciLexerJSON::Keyword, 0x8B8B00);   // 关键字

    SendScintilla(SCI_STYLESETFORE, QsciLexerJSON::String, 0x6666D4); // 字符串
    SendScintilla(SCI_STYLESETFORE, QsciLexerJSON::Operator, 0xB48246); // 运算符
    SendScintilla(SCI_STYLESETFORE, QsciLexerJSON::Number, 0x006F7F); // 数字
}

// 设置语法高亮
void TextEdit::setHighLight(const QString &language)
{
    if (language == "C/C++") {
        highlightCpp();
    } else if (language == "Bash") {
        highlightBash();
    } else if (language == "Lua") {
        highlightLua();
    } else if (language == "JSON") {
        highlightJSON();
    } else {
        highlightNone();
    }
    m_language = language;

    // 当前行高亮
    SendScintilla(SCI_SETCARETLINEVISIBLE, true);
    SendScintilla(SCI_SETCARETLINEBACK, 0xE0E0E0);
}

void TextEdit::setIndentationsUseTabs(bool enable)
{
    QsciScintilla::setIndentationsUseTabs(enable);
}

void TextEdit::setTabWidth(int width)
{
    QsciScintilla::setTabWidth(width);
}

void TextEdit::setAutoIndent(bool enable)
{
    QsciScintilla::setAutoIndent(enable);
}

void TextEdit::setIndentationGuides(bool enable)
{
    QsciScintilla::setIndentationGuides(enable);
}
