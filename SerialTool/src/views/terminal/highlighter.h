#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QJsonValue;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);
    void loadRule(const QString &fileName);

protected:
    void highlightBlock(const QString &text) override;

private:
    void appendRule(const QJsonValue &value);

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_highlightingRules;
};

#endif // HIGHLIGHTER_H
