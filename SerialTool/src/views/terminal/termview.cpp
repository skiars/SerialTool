#include "termview.h"
#include <QTextBlock>

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
        if (m_enabled) {
            emit enterNewline(inputText());
        } else {
            return;
        }
        m_lastPostion = textCursor().position() + 1;
        break;
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
    int end = cursor.position();
    cursor.setPosition(m_lastPostion, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    return cursor.selectedText() + '\n';
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
