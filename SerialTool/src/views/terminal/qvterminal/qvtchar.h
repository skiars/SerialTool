#ifndef QVTCHAR_H
#define QVTCHAR_H

#include "qvtcharformat.h"

class QVTChar
{
public:
    QVTChar();
    QVTChar(QChar c, const QVTCharFormat &format);

    QChar c() const;
    void setC(QChar c);

    QColor background() const;
    QColor foreground() const;

protected:
    QChar m_char;
    QColor m_background, m_foreground;
};

#endif // QVTCHAR_H
