#ifndef TERMVIEW_H
#define TERMVIEW_H

#include <QPlainTextEdit>

class Highlighter;

class TermView : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TermView(QWidget *parent = nullptr);
    void append(const QString &string);
    void clear();
    void setEnabled(bool enabled);

signals:
    void enterNewline(const QString &string);

private slots:
    void onCursorPosChanged();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    enum HistoryMove {
        Prev,
        Next
    };
    QString inputText();
    void loadHistory(HistoryMove mode = Prev);
    void selectAll();
    void moveHome();
    void scrollToBottom();
    void sendLine();

private:
    int m_lastPostion = 0, m_historyPos = 0;
    QVector<QString> m_history;
    Highlighter *m_highlight;
    QString m_input;
    int m_enabled = false;
};

#endif // TERMVIEW_H
