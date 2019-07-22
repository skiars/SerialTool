#include "qvtline.h"

QVTLine::QVTLine()
{
}

void QVTLine::append(const QVTChar &c, int position)
{
    if (position < 0) {
        _chars.append(c);
    } else if (position >= _chars.count()) {
        QVTChar spc = c;
        spc.setC(' ');
        for (int w = position - _chars.count(); w > 0; --w) {
            _chars.append(spc);
        }
        _chars.append(c);
    } else {
        _chars[position] = c;
    }
}

const QVector<QVTChar> &QVTLine::chars() const
{
    return _chars;
}

void QVTLine::reserve(int size)
{
    _chars.resize(size);
}

int QVTLine::size() const
{
    return _chars.size();
}
