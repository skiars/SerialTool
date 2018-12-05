#ifndef TERMVIEW_H
#define TERMVIEW_H

#include <QPlainTextEdit>

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
    QString inputText();

private:
    int m_lastPostion = 0;
    bool m_enabled = false;
};

#endif // TERMVIEW_H
