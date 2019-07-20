#include "termview.h"
#include <QTextBlock>
#include <QScrollBar>
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>

TermView::TermView(QWidget *parent) : QPlainTextEdit(parent)
{
    m_lastPostion = 0;
    setWordWrapMode(QTextOption::WrapAnywhere);
    connect(this, SIGNAL(cursorPositionChanged()), SLOT(onCursorPosChanged()) );
}

void TermView::onCursorPosChanged()
{
    setReadOnly(textCursor().position() < m_lastPostion);
}

void TermView::keyPressEvent(QKeyEvent *event)
{
    QString str;
    switch (event->key()) {
    case Qt::Key_Backspace:
        str += "\x08";
        break;
    case Qt::Key_Left:
        str += "\x1B\x5B\x44";
        break;
    case Qt::Key_Right:
        str += "\x1B\x5B\x43";
        break;
    case Qt::Key_Up:
        str += "\x1B\x5B\x41";
        break;
    case Qt::Key_Down:
        str += "\x1B\x5B\x42";
        break;
    case Qt::Key_Enter:
        break;
    case Qt::Key_Return:
        str += "\r";
        break;
    default:
        str += event->text();
        break;
    }
    emit enterNewline(str);
}

void TermView::append(const QString &string)
{
    insertPlainText(string);
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
    cursor.setPosition(m_lastPostion, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
}

void TermView::scrollToBottom()
{
    QScrollBar *scroll = verticalScrollBar();
    scroll->setValue(scroll->maximum());
}

void TermView::sendLine()
{
    QPlainTextEdit::appendPlainText("");
    scrollToBottom();
    if (m_historyPos < m_history.size()) {
        emit enterNewline(m_history[m_historyPos] + "\r\n");
    } else {
        emit enterNewline(m_input + "\r\n");
    }
    if (!m_input.isEmpty()) {
        m_history.append(m_input);
    }
    m_historyPos = m_history.size();
    m_input.clear();
}

void TermView::sendString(const QString &string)
{
    emit enterNewline(string);
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
