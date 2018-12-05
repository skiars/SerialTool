#include "termview.h"
#include <QTextBlock>
#include <QDebug>

TermView::TermView(QWidget *parent) : QPlainTextEdit(parent)
{
    m_lastPostion = 0;
    connect(this, SIGNAL(cursorPositionChanged()), SLOT(onCursorPosChanged()) );
}

void TermView::onCursorPosChanged()
{
    setReadOnly(textCursor().position() < m_lastPostion);
}

void TermView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (m_enabled) {
            QPlainTextEdit::keyPressEvent(event);
            emit enterNewline(inputText());
        } else {
            return;
        }
        m_lastPostion = textCursor().position();
        return;
    case Qt::Key_Backspace:
        if (m_lastPostion >= textCursor().position()) {
            return;
        }
        break;
    default:
        break;
    }
    QPlainTextEdit::keyPressEvent(event);
}

void TermView::append(const QString &string)
{
    moveCursor(QTextCursor::End);
    insertPlainText(string);
    m_lastPostion = toPlainText().length();
}

QString TermView::inputText()
{
    QTextCursor cursor = textCursor();
    int end = cursor.position() - 1; /* skip EOF */
    cursor.setPosition(m_lastPostion, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    return cursor.selectedText() + "\r\n";
}

void TermView::clear()
{
    m_lastPostion = 0;
    QPlainTextEdit::clear();
}

void TermView::setEnabled(bool enabled)
{
    m_enabled = enabled;
}
