#include "plotview.h"
#include <QChart>
#include <QValueAxis>
#include <qDebug>

QT_CHARTS_USE_NAMESPACE

PlotView::PlotView(QWidget *parent) : QtCharts::QChartView(parent)
{

}

int PlotView::xTickCount(int width)
{
    int i, tab[] = { 10, 5, 4 };

    for (i = 0; i < (int)sizeof(tab) - 1 && width / tab[i] < 80; ++i);
    return tab[i] + 1;
}

int PlotView::yTickCount(int height)
{
    int i, tab[] = { 10, 5, 4 };

    for (i = 0; i < (int)sizeof(tab) - 1 && height / tab[i] < 50; ++i);
    return tab[i] + 1;
}

void PlotView::resizeEvent(QResizeEvent *event)
{
    QValueAxis *xAxis = (QValueAxis *)chart()->axisX();
    QValueAxis *yAxis = (QValueAxis *)chart()->axisY();

    QtCharts::QChartView::resizeEvent(event);
    xAxis->setTickCount(xTickCount(event->size().width()));
    yAxis->setTickCount(yTickCount(event->size().height()));
    xAxis->setMinorTickCount(4);
    yAxis->setMinorTickCount(4);
}
