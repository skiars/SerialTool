#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include <QtCharts/QChartView>

class PlotView : public QtCharts::QChartView
{
public:
    PlotView(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    int xTickCount(int width);
    int yTickCount(int height);
};

#endif // PLOTVIEW_H
