#include "qvtcharformat.h"

QVTCharFormat::QVTCharFormat()
{
    QFont font;
    font.setFamily("monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(10);
    setFont(font);
    setForeground(QColor(187, 187, 187));
    setBackground(QColor(0, 0, 0));
}

QFont* QVTCharFormat::font()
{
    return &_font;
}

void QVTCharFormat::setFont(const QFont &font)
{
    _font = font;
}

const QColor &QVTCharFormat::foreground() const
{
    return _foreground;
}

void QVTCharFormat::setForeground(const QColor &foreground)
{
    _foreground = foreground;
}

const QColor &QVTCharFormat::background() const
{
    return _background;
}

void QVTCharFormat::setBackground(const QColor &background)
{
    _background = background;
}
