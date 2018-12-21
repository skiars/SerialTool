#include "highlighter.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    loadRule("./config/highlight/berry.json");
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : m_highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
}

void Highlighter::appendRule(const QJsonValue &value)
{
    if (value.isObject()) {
        QJsonObject obj(value.toObject());
        HighlightingRule rule;
        QTextCharFormat format;
        format.setFontItalic(obj.value("font-style").toString() == "italic");
        format.setForeground(QColor(obj.value("color").toString()));
        rule.pattern = QRegularExpression(obj.value("pattern").toString());
        rule.format = format;
        m_highlightingRules.append(rule);
    }
}

void Highlighter::loadRule(const QString &fileName)
{
    m_highlightingRules.clear();
    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QJsonParseError jsonError;
        QJsonDocument docment = QJsonDocument::fromJson(file.readAll(), &jsonError);
        file.close();
        if (jsonError.error == QJsonParseError::NoError && docment.isObject()) {
            QJsonObject object = docment.object();
            QJsonValue value = object.value("rules");
            if (value.isArray()) {
                QJsonArray array = value.toArray();
                for (QJsonValue value : array) {
                    appendRule(value);
                }
            }
        }
    }
}
