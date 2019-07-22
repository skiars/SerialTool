#include "qvtchar.h"

QVTChar::QVTChar()
{
    m_char = QChar();
}

QVTChar::QVTChar(QChar c, const QVTCharFormat &format)
{
    m_char = c;
    m_background = format.background();
    m_foreground = format.foreground();
}

QChar QVTChar::c() const
{
    return m_char;
}

void QVTChar::setC(QChar c)
{
    m_char = c;
}

QColor QVTChar::background() const
{
    return m_background;
}

QColor QVTChar::foreground() const
{
    return m_foreground;
}
