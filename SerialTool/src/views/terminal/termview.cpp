#include "termview.h"
#include <QTextBlock>
#include <QScrollBar>
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
    case Qt::Key_Up:
        loadHistory(Prev);
        return;
    case Qt::Key_Down:
        loadHistory(Next);
        return;
    case Qt::Key_Left:
        if (textCursor().position() <= m_lastPostion) {
            return;
        }
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (m_enabled) {
            QPlainTextEdit::appendPlainText("");
            appendHistory(m_input);
            scrollToBottom();
            emit enterNewline(m_input + "\r\n");
            m_input.clear();
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
    case Qt::Key_A:
        if (event->modifiers() == Qt::ControlModifier) {
            selectAll();
            return;
        }
        break;
    case Qt::Key_Home:
        break;
    default:
        break;
    }
    QPlainTextEdit::keyPressEvent(event);
    m_input = inputText();
}

void TermView::append(const QString &string)
{
    moveCursor(QTextCursor::End);
    insertPlainText(string);
    m_lastPostion = toPlainText().length();
    scrollToBottom();
    m_input.clear();
}

void TermView::appendHistory(const QString &string)
{
    m_history.append(string);
    m_historyPos = m_history.size();
}

QString TermView::inputText()
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_lastPostion, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    return cursor.selectedText();
}

void TermView::loadHistory(HistoryMove mode)
{
    QTextCursor cursor = textCursor();
    if (m_lastPostion < cursor.position()) {
        cursor.setPosition(m_lastPostion, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
    }
    if (mode == Prev) {
        if (m_historyPos > 0) {
            --m_historyPos;
        }
    } else {
        if (m_historyPos < m_history.size()) {
            ++m_historyPos;
        }
        if (m_historyPos == m_history.size()) {
            insertPlainText(m_input);
            return;
        }
    }
    insertPlainText(m_history[m_historyPos]);
}

void TermView::selectAll()
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_lastPostion, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

void TermView::moveHome()
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_lastPostion + 1, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
}

void TermView::scrollToBottom()
{
    QScrollBar *scroll = verticalScrollBar();
    scroll->setValue(scroll->maximum());
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
