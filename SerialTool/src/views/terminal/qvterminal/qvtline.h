#ifndef QVTLINE_H
#define QVTLINE_H

#include "qvtchar.h"
#include <QVector>

class QVTLine
{
public:
    QVTLine();

    void append(const QVTChar &c, int position = -1);
    void reduce(int position);
    void reserve(int size);
    const QVector<QVTChar> &chars() const;
    int size() const;

protected:
    QVector<QVTChar> _chars;
};

#endif // QVTLINE_H
