#include "oscilloscope.h"

#define SCALE   (1000.0 / _xRange)

Oscilloscope::Oscilloscope(QWidget *parent)
{
    ui.setupUi(parent);

    setupPlot();
    setupChannel();
    listViewInit();

    QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui.xRangeBox->lineEdit()->setValidator(pReg);

    updataTimer.setInterval(15);
    updataTimer.start();

    connect(ui.customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui.customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    connect(ui.horizontalScrollBar, &QAbstractSlider::valueChanged, this, &Oscilloscope::horzScrollBarChanged);
    connect(ui.yOffsetBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &Oscilloscope::yOffsetChanged);
    connect(ui.yRangeBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &Oscilloscope::yRangeChanged);
    connect(ui.xRangeBox, &QComboBox::currentTextChanged, this, &Oscilloscope::xRangeChanged);
    connect(&updataTimer, &QTimer::timeout, this, &Oscilloscope::timeUpdata);

    setXRange(10);
    clear();
}

// 重新设置语言
void Oscilloscope::retranslate()
{
    ui.retranslateUi(this);
}


// 初始化示波器界面
void Oscilloscope::setupPlot()
{
    // 拖动时不抗锯齿
    ui.customPlot->setNoAntialiasingOnDrag(true);

    // 设置刻度线
    QSharedPointer<	QCPAxisTicker> xTicker(new QCPAxisTicker);
    QSharedPointer<	QCPAxisTicker> yTicker(new QCPAxisTicker);
    xTicker->setTickCount(5);
    ui.customPlot->xAxis->setTicker(xTicker);
    ui.customPlot->xAxis2->setTicker(xTicker);
    yTicker->setTickCount(5);
    ui.customPlot->yAxis->setTicker(yTicker);
    ui.customPlot->yAxis2->setTicker(yTicker);
    // 显示小网格
    ui.customPlot->xAxis->grid()->setSubGridVisible(true);
    ui.customPlot->yAxis->grid()->setSubGridVisible(true);
}

// 初始化通道
void Oscilloscope::setupChannel()
{
    // 初始化通道
    for (int i = 0; i < CH_NUM; ++i) {
        count[i] = 0.0;
        ui.customPlot->addGraph();
    }

    ui.customPlot->axisRect()->setupFullAxesBox(true);
    ui.customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // 允许拖拽和缩放
    ui.customPlot->yAxis->setRange(0, 2, Qt::AlignCenter);
}

// 通道列表初始化
void Oscilloscope::listViewInit()
{
    ui.channelList->setModelColumn(2); // 两列
    for (int i = 0; i < CH_NUM; ++i) {
        QListWidgetItem *item = new QListWidgetItem;
        ui.channelList->addItem(item);
        ChannelItem *chItem = new ChannelItem("CH" + QString::number(i + 1));
        ui.channelList->setItemWidget(item, chItem);
        chItem->setChannel(i);
        channelStyleChanged(chItem);
        connect(chItem, &ChannelItem::changelChanged, this, &Oscilloscope::channelStyleChanged);
    }
}

// 返回Y轴偏置
double Oscilloscope::yOffset()
{
    return ui.yOffsetBox->value();
}

// 返回Y轴范围
double Oscilloscope::yRange()
{
    return ui.yRangeBox->value();
}

// 返回X轴范围
double Oscilloscope::xRange()
{
    return ui.customPlot->xAxis->range().size();
}

// 设置Y轴偏置
void Oscilloscope::setYOffset(double offset)
{
    ui.yOffsetBox->setValue(offset);
}

// 设置Y轴范围
void Oscilloscope::setYRange(double range)
{
    ui.yRangeBox->setValue(range);
}

// 返回通道是否可见
bool Oscilloscope::channelVisible(int channel)
{
    ChannelItem *item = (ChannelItem *)(
        ui.channelList->itemWidget(ui.channelList->item(channel)));

    return item->isChecked();
}

// 返回通道颜色
QColor Oscilloscope::channelColor(int channel)
{
    ChannelItem *item = (ChannelItem *)(
        ui.channelList->itemWidget(ui.channelList->item(channel)));

    return item->color();
}

// 返回更新定时器
QTimer * Oscilloscope::timer()
{
    return &updataTimer;
}

// 返回保持接收状态
bool Oscilloscope::holdReceive()
{
    return ui.holdReceiveBox->isChecked();
}

// 设置X轴点数
void Oscilloscope::setXRange(double range)
{
    setXRange(QString::number(range));
}

// 设置X轴点数
void Oscilloscope::setXRange(const QString &str)
{
    _xRange = str.toDouble();
    ui.xRangeBox->setCurrentText(str);
}

// 设置波形绘制抗锯齿
void Oscilloscope::setPlotAntialiased(bool status)
{
    ui.customPlot->setNotAntialiasedElement(QCP::aePlottables, !status);
}

// 设置网格抗锯齿
void Oscilloscope::setGridAntialiased(bool status)
{
    ui.customPlot->setAntialiasedElement(QCP::aeGrid, status);
    ui.customPlot->setAntialiasedElement(QCP::aeAxes, status);
}

// 设置背景颜色
void Oscilloscope::setBackground(QColor color)
{
    ui.customPlot->setBackground(QBrush(color));
}

// 设置网格和图例颜色
void Oscilloscope::setGridColor(QColor color)
{
    QPen pen(color);

    ui.customPlot->xAxis->setBasePen(pen);
    ui.customPlot->xAxis->setTickPen(pen);
    ui.customPlot->xAxis->grid()->setPen(pen);
    ui.customPlot->xAxis->setSubTickPen(pen);
    ui.customPlot->xAxis->setTickLabelColor(color);
    ui.customPlot->xAxis->setLabelColor(color);
    ui.customPlot->xAxis->grid()->setZeroLinePen(pen); // 零点画笔
    ui.customPlot->yAxis->setBasePen(pen);
    ui.customPlot->yAxis->setTickPen(pen);
    ui.customPlot->yAxis->grid()->setPen(pen);
    ui.customPlot->yAxis->setSubTickPen(pen);
    ui.customPlot->yAxis->setTickLabelColor(color);
    ui.customPlot->yAxis->setLabelColor(color);
    ui.customPlot->yAxis->grid()->setZeroLinePen(pen); // 零点画笔
    ui.customPlot->xAxis2->setBasePen(pen);
    ui.customPlot->xAxis2->setTickPen(pen);
    ui.customPlot->xAxis2->setSubTickPen(pen);
    ui.customPlot->yAxis2->setBasePen(pen);
    ui.customPlot->yAxis2->setTickPen(pen);
    ui.customPlot->yAxis2->setSubTickPen(pen);

    // 网格颜色
    color.setAlpha(0x30);
    pen.setColor(color);
    ui.customPlot->xAxis->grid()->setSubGridPen(pen);
    ui.customPlot->yAxis->grid()->setSubGridPen(pen);
}

// 设置通道颜色
void Oscilloscope::setChannelColor(int chanel, const QColor &color)
{
    ChannelItem *item = (ChannelItem *)(
        ui.channelList->itemWidget(ui.channelList->item(chanel)));
    item->setColor(color);
    ui.customPlot->graph(chanel)->setPen(QPen(item->color()));
}

// 设置通道是否可见
void Oscilloscope::setChannelVisible(int chanel, bool visible)
{
    ChannelItem *item = (ChannelItem *)(
        ui.channelList->itemWidget(ui.channelList->item(chanel)));
    item->setChecked(visible);
    ui.customPlot->graph(chanel)->setVisible(visible);
}

// 添加数据
void Oscilloscope::addData(int channel, double value)
{
    ui.customPlot->graph(channel)->addData(count[channel], value);
    count[channel] += 1.0;
}

// 清空数据
void Oscilloscope::clear()
{
    for (int i = 0; i < CH_NUM; ++i) {
        ui.customPlot->graph(i)->data()->clear();
        count[i] = 0.0;
    }
    ui.horizontalScrollBar->setValue(0);
    ui.horizontalScrollBar->setRange(0, 0);
    ui.customPlot->xAxis->setRange(0, _xRange, Qt::AlignLeft);
    ui.customPlot->replot();
}

// 保存PNG文件
void Oscilloscope::savePng(const QString &fileName)
{
    ui.customPlot->savePng(fileName);
}

// 保存BMP文件
void Oscilloscope::saveBmp(const QString &fileName)
{
    ui.customPlot->saveBmp(fileName);
}

// 保存PDF文件
void Oscilloscope::savePdf(const QString &fileName)
{
    ui.customPlot->savePdf(fileName);
}

// 通道显示风格改变
void Oscilloscope::channelStyleChanged(ChannelItem *item)
{
    int ch = item->channel();
    ui.customPlot->graph(ch)->setVisible(item->isChecked());
    ui.customPlot->graph(ch)->setPen(QPen(item->color()));
    ui.customPlot->replot();
}

// x轴发生变化
void Oscilloscope::xAxisChanged(QCPRange range)
{
    // 设置x轴范围
    if (_xRange != range.size()) {
        _xRange = range.size();
        ui.xRangeBox->setEditText(QString::number(_xRange));
    }
    // 设置滚动条
    if (qAbs(range.lower * SCALE - ui.horizontalScrollBar->value()) > 1.0 / SCALE) {
        int key = qRound(range.lower * SCALE);
        ui.horizontalScrollBar->setValue(key);
        replotFlag = false;
    }
}

// y轴发生变化
void Oscilloscope::yAxisChanged(QCPRange range)
{
    // 只有当用户拖拽customPlot控件时才会设置偏移值
    if (range.center() != ui.yOffsetBox->value()) {
        ui.yOffsetBox->setValue(range.center());
    }
    // 只有当用户拖拽customPlot控件时才会设置偏移值
    if (range.size() != ui.yRangeBox->value()) {
        ui.yRangeBox->setValue(range.size());
    }
}

// 滚动条滑块移动时触发
void Oscilloscope::horzScrollBarChanged(int value)
{
    if (ui.horizontalScrollBar->maximum() == value) {
        replotFlag = true;
    } else {
        replotFlag = false;
    }
    if (ui.customPlot->xAxis->range().lower < value / SCALE || ui.horizontalScrollBar->maximum() != ui.horizontalScrollBar->value()) {
        ui.customPlot->xAxis->setRange(value / SCALE,
            _xRange, Qt::AlignLeft);
    }
}

// Y轴偏置改变
void Oscilloscope::yOffsetChanged(double offset)
{
    double range = ui.customPlot->yAxis->range().size();
    ui.customPlot->yAxis->setRange(offset, range, Qt::AlignCenter);
    ui.customPlot->replot();
}

// Y轴范围改变
void Oscilloscope::yRangeChanged(double range)
{
    double offset = ui.customPlot->yAxis->range().center();
    ui.customPlot->yAxis->setRange(offset, range, Qt::AlignCenter);
    ui.customPlot->replot();
}

// X轴范围改变
void Oscilloscope::xRangeChanged(const QString &str)
{
    double upper = ui.customPlot->xAxis->range().upper;

    _xRange = str.toDouble();
    if (upper < _xRange) {
        ui.horizontalScrollBar->setRange(0, 0);
        ui.horizontalScrollBar->setValue(0);
        ui.customPlot->xAxis->setRange(0, _xRange);
    } else {
        ui.horizontalScrollBar->setRange(0, (int)((upper - _xRange) * SCALE));
        ui.horizontalScrollBar->setValue((int)((upper - _xRange) * SCALE));
        ui.customPlot->xAxis->setRange(upper, _xRange, Qt::AlignRight);
    }
    ui.horizontalScrollBar->setPageStep(_xRange * SCALE);
    ui.customPlot->replot();
}

// 更新定时器触发
void Oscilloscope::timeUpdata()
{
    // 显示更新
    double key = count[0];
    for (int i = 0; i < CH_NUM; ++i) {
        key = key > count[i] ? key : count[i];
    }
    if (key > _xRange) {
        ui.horizontalScrollBar->setRange(0, (int)((key - _xRange) * SCALE));
        if (replotFlag || key <= ui.customPlot->xAxis->range().upper) {
            ui.horizontalScrollBar->setValue((int)((key - _xRange) * SCALE));
        }
    }
    ui.customPlot->replot();
}
