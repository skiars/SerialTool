#ifndef QVTCHARFORMAT_H
#define QVTCHARFORMAT_H

#include <QColor>
#include <QFont>

class QVTCharFormat
{
public:
    QVTCharFormat();

    QFont* font();
    void setFont(const QFont &font);

    const QColor &foreground() const;
    void setForeground(const QColor &foreground);

    const QColor &background() const;
    void setBackground(const QColor &background);

protected:
    QFont _font;
    QColor _foreground;
    QColor _background;
};

#endif // QVTCHARFORMAT_H
