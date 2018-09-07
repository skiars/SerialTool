#include "pointdatabuffer.h"
#include <QVector>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

PointDataBuffer::PointDataBuffer(const QVector<QLineSeries*> *series)
{
    m_series = series;
    m_data.resize(series->size());
    m_count.resize(series->size());
    clear();
}

void PointDataBuffer::append(int channel, double value)
{
    m_data[channel].append(QPointF(m_count[channel]++, value));
}

int PointDataBuffer::update()
{
    int size = m_data.size();

    for (int i = 0; i < size; ++i) {
        if (m_data[i].size()) {
            m_series->at(i)->replace(m_data[i]);
        }
    }
    return maximumCount();
}

int PointDataBuffer::maximumCount()
{
    int max = 0;
    int size = m_count.size();

    for (int i = 0; i < size; ++i) {
        if (m_count[i] > max) {
            max = m_count[i];
        }
    }
    return max;
}

void PointDataBuffer::clear()
{
    int size = m_data.size();

    for (int i = 0; i < size; ++i) {
        m_series->at(i)->clear(); // clear series data
        m_data[i].clear(); // clear buffer
        m_count[i] = 0; // clear count
    }
}
