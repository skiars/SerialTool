#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QTextCharFormat format;
    QString gkey("None|String|Int|Float|Bool"
                 "|[Kk]eyword|hidden|conditional|referencealias"
                 "|self|armor|key|game|idle"
                 "|playerref|anims|tmp|name|var|Version|Angle"
                 "|[wW]ait"
                 "|[eE][rR][rR][oo][rR]"
                 "|(Get|Set|Reset|Clear|Add|On|[sS]how|[cC]lear)[A-Z]\\w*" // prefix
                 "|\\w*([lL]ibs?(_\\w+)?|(_vV)ar|Event|Rendered|[Kk]eys?|Filters?|ID|minsize|maxsize|Solo|[bB]ox)" // suffix
                 "|\\w*(queststart|Unlock|End|Menu|Magic|Remove|Notify" // infix
                      "|Player|[fF]action|[cC]rime|Name|Valid|Disable|Inventory|Gold|[pP]erk"
                      "|Thief|[fF]orm|Armor|Select|Cast|Delete|[hH]ealth|lockstate|origin"
                      "|Save|mindays|maxdays|Difficulty|File|Create|Weapon|Continue|[lL]ocked"
                      "|Activate|Rank|[mM]essage|Update|Controls|[eE]nable|[dD]isable|Stop|Apply|Effect)\\w*"
                 "|((\\w*A)|a)ctor(([_A-Z]\\w*)|((base|util)([_A-Z]\\w*)?))?" // Actor
                 "|\\w*(O|_?o)ut[Ff]it\\w*" // outfit
                 "|(\\w+_m|\\w*M|m)[sS][gG]([_A-Z]\\w*)?" // msg
                 "|(\\w+_s|\\w*S|s)tart\\w*" // start
                 "|(\\w+_s|\\w*C|c)ontainer([_A-Z]\\w*)?" // start
                 "|\\w*[wW]eight" // weight
                 "");
    QString lkey("Function|Event|\\w*[Ss]tate|\\w*[Ss]cript|[Qq]uest");

    // invalid foramt
    format.setFontItalic(false);
    format.setForeground(Qt::black);
    format.setUnderlineColor(Qt::red);
    format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    rule.pattern = QRegularExpression("[A-Z]*([a-zA-Z]+[_0-9]+|[a-z]+[A-Z]+)\\w*");
    rule.format = format;
    highlightingRules.append(rule);

    format.setFontItalic(true);
    format.setForeground(Qt::magenta);
    format.setUnderlineStyle(QTextCharFormat::NoUnderline);
    rule.pattern = QRegularExpression("(^(" + gkey + ")$)|(\\b(" + lkey + ")\\b)");
    rule.format = format;
    highlightingRules.append(rule);

    format.setFontItalic(true);
    format.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("::.*$");
    rule.format = format;
    highlightingRules.append(rule);

    format.setFontItalic(true);
    format.setForeground(Qt::darkCyan);
    rule.pattern = QRegularExpression("^(\\$.*|\\w*\\[])$");
    rule.format = format;
    highlightingRules.append(rule);

    format.setFontItalic(true);
    format.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\\[.+]");
    rule.format = format;
    highlightingRules.append(rule);

    format.setFontItalic(true);
    format.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(".*\\.(esp|esm)");
    rule.format = format;
    highlightingRules.append(rule);

    format.setFontItalic(true);
    format.setForeground(Qt::darkGray);
    rule.pattern = QRegularExpression("\\b[0-9]+(\\.[0-9]*)?\\b");
    rule.format = format;
    highlightingRules.append(rule);

    format.setFontItalic(false);
    format.setForeground(Qt::black);
    format.setBackground(QColor("#9BDD9B"));
    findRule.pattern = QRegularExpression("");
    findRule.format = format;
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    QRegularExpressionMatchIterator matchIterator = findRule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), findRule.format);
    }
    setCurrentBlockState(0);
}

void Highlighter::setFind(const QRegularExpression &regExp)
{
    findRule.pattern = regExp;
    rehighlight();
}
