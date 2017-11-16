#include "oscilloscope.h"
#include <QTextStream>
#include <QSettings.h>
#include "channelitem.h"
#include "wavedecode.h"
#include "oscopetimestamp.h"

#define SCALE   (1000.0 / _xRange)

Oscilloscope::Oscilloscope(QWidget *parent)
{
    ui.setupUi(parent);

    timeStamp = new OscopeTimeStamp();
    _xRange = 0;

    setupPlot();
    setupChannel();
    listViewInit();

    QRegExpValidator *pReg = new QRegExpValidator(QRegExp("^\\d{2,7}$"));
    ui.xRangeBox->lineEdit()->setValidator(pReg);

    updataTimer.setInterval(25);

    connect(ui.customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui.customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    connect(ui.horizontalScrollBar, &QAbstractSlider::valueChanged, this, &Oscilloscope::horzScrollBarChanged);
    connect(ui.yOffsetBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &Oscilloscope::yOffsetChanged);
    connect(ui.yRangeBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &Oscilloscope::yRangeChanged);
    connect(ui.xRangeBox, &QComboBox::currentTextChanged, this, &Oscilloscope::xRangeChanged);
    connect(&updataTimer, &QTimer::timeout, this, &Oscilloscope::timeUpdata);

    clear();
    ui.channelList->setVisible(false);
}

Oscilloscope::~Oscilloscope()
{
    delete timeStamp;
}

// 重新设置语言
void Oscilloscope::retranslate()
{
    ui.retranslateUi(this);
}

// load settings
void Oscilloscope::loadConfig(QSettings *config)
{
    config->beginGroup("Oscillograph");
    ui.xRangeBox->setCurrentText(config->value("XRange").toString());
    ui.yOffsetBox->setValue(config->value("YOffset").toDouble());
    ui.yRangeBox->setValue(config->value("YRange").toDouble());
    ui.holdReceiveBox->setChecked(config->value("HoldReceive").toBool());
    // load channels settings
    config->beginReadArray("Channels");
    for (int i = 0; i < CH_NUM; ++i) {
        config->setArrayIndex(i);
        setChannelVisible(i, config->value("Visible").toBool());
        QColor color(config->value("Color").toString());
        ChannelItem *item = channelWidget(i);
        item->setColor(color);
        ui.customPlot->graph(i)->setPen(QPen(color));
    }
    config->endArray();
    config->endGroup();
}

// save settings
void Oscilloscope::saveConfig(QSettings *config)
{
    config->beginGroup("Oscillograph");
    config->setValue("YOffset", QVariant(ui.yOffsetBox->value()));
    config->setValue("YRange", QVariant(ui.yRangeBox->value()));
    config->setValue("XRange", QVariant(ui.xRangeBox->currentText()));
    config->setValue("HoldReceive", QVariant(ui.holdReceiveBox->isChecked()));
    // save channels settings
    config->beginWriteArray("Channels");
    for (int i = 0; i < CH_NUM; ++i) {
        config->setArrayIndex(i);
        config->setValue("Visible", channelWidget(i)->isChecked());
        config->setValue("Color", channelWidget(i)->color().name());
    }
    config->endArray();
    config->endGroup();
}

// 初始化示波器界面
void Oscilloscope::setupPlot()
{
    // 拖动时不抗锯齿
    ui.customPlot->setNoAntialiasingOnDrag(true);

    // 设置刻度线
    QSharedPointer<QCPAxisTicker> xTicker(new QCPAxisTicker);
    QSharedPointer<QCPAxisTicker> yTicker(new QCPAxisTicker);
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
        count[i] = 0;
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
    ui.channelList->editItem(ui.channelList->item(0));
}

// 开始运行
void Oscilloscope::start()
{
    updataTimer.start();
}

// 结束运行
void Oscilloscope::stop()
{
    updataTimer.stop();
}

// 返回保持接收状态
bool Oscilloscope::holdReceive()
{
    return ui.holdReceiveBox->isChecked();
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
void Oscilloscope::setChannelVisible(int channel, bool visible)
{
    channelWidget(channel)->setChecked(visible);
    ui.customPlot->graph(channel)->setVisible(visible);
}

// 添加数据
void Oscilloscope::addData(const WaveDataType& data)
{
    if (data.mode == WaveValueMode) {
        uint8_t channel = data.channel;

        ui.customPlot->graph(channel)->addData(count[channel], data.value);
        count[channel] += 1;
    } else { // WaveTimeStampMode
        timeStamp->append(data, maxCount());
    }
}

// 清空数据
void Oscilloscope::clear()
{
    for (int i = 0; i < CH_NUM; ++i) {
        ui.customPlot->graph(i)->data()->clear();
        count[i] = 0;
    }
    timeStamp->clear();
    ui.horizontalScrollBar->setValue(0);
    ui.horizontalScrollBar->setRange(0, 0);
    ui.customPlot->xAxis->setRange(0, _xRange, Qt::AlignLeft);
    ui.customPlot->replot();
}

uint64_t Oscilloscope::maxCount()
{
    uint64_t max = 0;

    for (int i = 0; i < CH_NUM; ++i) {
        if (count[i] > max) {
            max = count[i];
        }
    }
    return max;
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
    uint64_t key = count[0];
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

// 保存txt文件
void Oscilloscope::saveText(const QString &fname)
{
    QFile file(fname);
    uint64_t dataCountMax = maxCount();

    file.open(QFile::WriteOnly);
    QTextStream out(&file);

    out << "Index";
    for (int i = 0; i < CH_NUM; ++i) {
        if (count[i]) {
            out << ", CH" << i + 1;
        }
    }
    out << endl;

    out.setRealNumberPrecision(8);
    for (uint64_t i = 0; i < dataCountMax; ++i) {
        timeStamp->printTextStream(out, i);
        out << i;
        for (int j = 0; j < CH_NUM; ++j) {
            if (i < count[j]) {
                double value = ui.customPlot->graph(j)->dataMainValue(i);
                out << ", " << value;
            }
        }
        out << endl;
    }
    file.close();
}
