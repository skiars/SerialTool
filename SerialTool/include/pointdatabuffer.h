#ifndef POINTDATABUFFER_H
#define POINTDATABUFFER_H

#include <QVector>
#include <QList>
#include <QPointF>

namespace QtCharts {
class QLineSeries;
}

class PointDataBuffer
{
public:
    PointDataBuffer(const QVector<QtCharts::QLineSeries*> *series);
    void append(int channel, double value);
    int update();
    int maximumCount();
    void clear();

private:
    const QVector<QtCharts::QLineSeries*> *m_series;
    QVector<QList<QPointF>> m_data;
    QVector<int> m_count;
};

#endif // POINTDATABUFFER_H
