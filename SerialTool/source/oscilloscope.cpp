#include "oscilloscope.h"
#include <QTextStream>
#include <QSettings>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QLineEdit>
#include <QScreen>
#include <QMessageBox>
#include "channelitem.h"
#include "wavedecode.h"
#include "oscopetimestamp.h"
#include "pointdatabuffer.h"

#define SCALE   (1000.0 / _xRange)

QT_CHARTS_USE_NAMESPACE

Oscilloscope::Oscilloscope(QWidget *parent) :
    QWidget(parent),
    m_chart(0)
{
    m_chart = new QChart;
    ui.setupUi(parent);

    timeStamp = new OscopeTimeStamp();
    m_xRange = 0;

    setupPlot();
    listViewInit();

    QRegExpValidator *pReg = new QRegExpValidator(QRegExp("^\\d{2,7}$"));
    ui.xRangeBox->lineEdit()->setValidator(pReg);

    updataTimer.setInterval(25);

    connect(ui.horizontalScrollBar, &QAbstractSlider::sliderMoved, this, &Oscilloscope::horzScrollBarChanged);
    connect(ui.yOffsetBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &Oscilloscope::yOffsetChanged);
    connect(ui.yRangeBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &Oscilloscope::yRangeChanged);
    connect(ui.xRangeBox, &QComboBox::currentTextChanged, this, &Oscilloscope::xRangeChanged);
    connect(&updataTimer, &QTimer::timeout, this, &Oscilloscope::timeUpdata);

    clear();
}

Oscilloscope::~Oscilloscope()
{
    delete timeStamp;
    delete m_chart;
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
    QString xRange = config->value("XRange").toString();
    double yRange = config->value("YRange").toDouble();
    double yOffset = config->value("YOffset").toDouble();
    ui.xRangeBox->setCurrentIndex(ui.xRangeBox->findText(xRange));
    ui.xRangeBox->setCurrentText(xRange);
    ui.yRangeBox->setValue(yRange);
    ui.yOffsetBox->setValue(yOffset);
    xRangeChanged(xRange);
    yRangeChanged(yRange);
    yOffsetChanged(yOffset);
    ui.holdReceiveBox->setChecked(config->value("HoldReceive").toBool());
    // load channels settings
    config->beginReadArray("Channels");
    for (int i = 0; i < CH_NUM; ++i) {
        config->setArrayIndex(i);
        QColor color(config->value("Color").toString());
        ChannelItem *item = channelWidget(i);
        item->setChecked(config->value("Visible").toBool());
        item->setColor(color);
        channelStyleChanged(item);
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
    ui.chartView->setChart(m_chart);
    m_chart->createDefaultAxes();
    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis;
    xAxis->setLabelFormat("%d");
    xAxis->setTickCount(6);
    xAxis->setMinorTickCount(1);
    yAxis->setTickCount(5);
    yAxis->setMinorTickCount(1);
    // 初始化通道
    for (int i = 0; i < CH_NUM; ++i) {
        QLineSeries *series = new QLineSeries(this);
        m_series.append(series);
        m_chart->addSeries(series);
        m_chart->setAxisX(xAxis, series);
        m_chart->setAxisY(yAxis, series);
    }
    m_chart->legend()->hide(); // 隐藏图例
    m_chart->setMargins(QMargins(0, 0, 0, 0)); // set margins
    m_chart->setContentsMargins(-9, -9, -8, -8);
    m_chart->setBackgroundRoundness(0);

    m_buffer = new PointDataBuffer(&m_series);
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

// 设置是否使用OpenGL加速
void Oscilloscope::setUseOpenGL(bool status)
{
    for (int i = 0; i < CH_NUM; ++i) {
        m_series[i]->setUseOpenGL(status);
    }
    if (status) {
        setUseAntialiased(false);
    }
}

// 设置是否使用抗锯齿
void Oscilloscope::setUseAntialiased(bool status)
{
    status &= !m_series[0]->useOpenGL(); // 不适用OpenGL时才可以打开抗锯齿
    ui.chartView->setRenderHint(QPainter::Antialiasing, status);
}

// 设置背景颜色
void Oscilloscope::setBackground(QColor color)
{
    m_chart->setBackgroundPen(QPen(color));
    m_chart->setBackgroundBrush(QBrush(color));
}

// 设置网格和图例颜色
void Oscilloscope::setGridColor(QColor color)
{
    m_chart->axisX()->setLinePen(QPen(color));
    m_chart->axisX()->setGridLineColor(color);
    m_chart->axisX()->setLabelsColor(color);
    m_chart->axisX()->setMinorGridLineColor(color);
    m_chart->axisY()->setLinePen(QPen(color));
    m_chart->axisY()->setGridLineColor(color);
    m_chart->axisY()->setLabelsColor(color);
    m_chart->axisY()->setMinorGridLineColor(color);
}

// 设置更新时间
void Oscilloscope::setUpdateInterval(int msec)
{
    updataTimer.setInterval(msec);
}

// 添加数据
void Oscilloscope::addData(const WaveDataType& data)
{
    if (data.mode == WaveValueMode) {
        m_buffer->append(data.channel, data.value); // 先将数据暂存到缓冲区
    } else { // Wave Time StampMode
        timeStamp->append(data, m_buffer->maximumCount());
    }
}

// 清空数据
void Oscilloscope::clear()
{
    m_buffer->clear();
    timeStamp->clear();
    ui.horizontalScrollBar->setMaximum(0);
    m_chart->axisX()->setRange(0, m_xRange);
}

// 保存PNG文件
void Oscilloscope::savePng(const QString &fileName)
{
    QScreen *screen = QGuiApplication::primaryScreen();

    QPixmap p = screen->grabWindow(ui.chartView->winId());
    QImage image = p.toImage();
    image.save(fileName);
}

// 保存BMP文件
void Oscilloscope::saveBmp(const QString &fileName)
{
    QScreen *screen = QGuiApplication::primaryScreen();

    QPixmap p = screen->grabWindow(ui.chartView->winId());
    QImage image = p.toImage();
    image.save(fileName);
}

// 通道显示风格改变
void Oscilloscope::channelStyleChanged(ChannelItem *item)
{
    int ch = item->channel();
    m_series[ch]->setVisible(item->isChecked());
    m_series[ch]->setPen(QPen(item->color(), 1.1));
}

// 滚动条滑块移动时触发
void Oscilloscope::horzScrollBarChanged(int value)
{
    if (ui.horizontalScrollBar->maximum() == value) {
        replotFlag = true;
    } else {
        replotFlag = false;
    }
    m_chart->axisX()->setRange(value, value + m_xRange);
}

// Y轴偏置改变
void Oscilloscope::yOffsetChanged(double offset)
{
    double range = ui.yRangeBox->value();

    m_chart->axisY()->setRange(offset - range * 0.5, offset + range * 0.5);
}

// Y轴范围改变
void Oscilloscope::yRangeChanged(double range)
{
    double offset = ui.yOffsetBox->value();

    m_chart->axisY()->setRange(offset - range * 0.5, offset + range * 0.5);
}

// X轴范围改变
void Oscilloscope::xRangeChanged(const QString &str)
{
    m_count = m_buffer->update();
    m_xRange = str.toDouble();
    ui.horizontalScrollBar->setPageStep(m_xRange);

    int count = m_count - 1;
    if (count > m_xRange) {
        int lower = count - m_xRange;
        m_chart->axisX()->setRange(lower, count);
        bool req = ui.horizontalScrollBar->value()
                == ui.horizontalScrollBar->maximum();
        ui.horizontalScrollBar->setMaximum(lower);
        if (req) {
            ui.horizontalScrollBar->setValue(lower);
        }
    } else {
        m_chart->axisX()->setRange(0, m_xRange);
        ui.horizontalScrollBar->setMaximum(0);
    }
}

// 更新定时器触发
void Oscilloscope::timeUpdata()
{

    int count = m_buffer->update();
    if (count > m_xRange + 1 && count > m_count) {
        if (replotFlag) {
            qreal dx = m_chart->plotArea().width() / m_xRange;
            dx *= count - m_count;
            m_chart->scroll(dx, 0);
        }
        // update scroll bar
        int lower = count - m_xRange - 1;
        bool req = ui.horizontalScrollBar->value()
                == ui.horizontalScrollBar->maximum();
        ui.horizontalScrollBar->setMaximum(lower);
        if (req) {
            ui.horizontalScrollBar->setValue(lower);
        }
    }
    m_count = count;
}

// 保存txt文件
void Oscilloscope::saveWave(const QString &fname)
{
    QFile file(fname);
    int dataCountMax = m_buffer->maximumCount();

    m_buffer->update();

    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    int count[CH_NUM];

    out << "Index";
    for (int i = 0; i < CH_NUM; ++i) {
        count[i] = m_series[i]->count();
        if (count[i]) {
            out << ", CH" << i + 1;
        }
    }
    out << endl;

    out.setRealNumberPrecision(8);
    for (int i = 0; i < dataCountMax; ++i) {
        timeStamp->printTextStream(out, i);
        out << i;
        for (int j = 0; j < CH_NUM; ++j) {
            if (i < count[j]) {
                double value = m_series[j]->at(i).y();
                out << ", " << value;
            }
        }
        out << endl;
    }
    file.close();
}

// 读取txt文件, 私有函数
bool Oscilloscope::loadWave_p(const QString &fname)
{
    bool ok = true;
    int channelCount = 0, lineCount = 0;
    int channelIndex[CH_NUM];

    clear();

    QFile file(fname);
    file.open(QFile::ReadOnly);
    QByteArray line = file.readLine();
    // load table header
    QStringList lineList = csvSplitLine(line);
    int listSize = lineList.size();
    for (int i = 1; i < listSize && ok; ++i) {
        QString str = lineList[i];
        if (str.mid(0, 2) == "CH") {
            int index = str.mid(2).toInt(&ok) - 1; // CH1 -> 0 ... CH16 -> 15
            if (ok && index < CH_NUM) {
                channelIndex[channelCount++] = index;
            } else {
                ok = false;
            }
        }
    }
    ok &= (channelCount < CH_NUM) & (channelCount + 1 == listSize);
    while (!file.atEnd() && ok) {
        line = file.readLine();
        lineList = csvSplitLine(line);
        if (lineList[0][0] == '#') { // time stamp
            timeStamp->append(lineList[0].mid(2), lineCount);
        } else if (lineList.size() == listSize) {
            if (lineList[0].toInt() == lineCount++) { // 行号检查
                for (int i = 1; i < listSize && ok; ++i) {
                    int channel = channelIndex[i - 1];
                    if (!lineList[i].isEmpty()) {
                        double value = lineList[i].toDouble(&ok);
                        m_buffer->append(channel, value);
                    }
                }
            } else {
                ok = false;
            }
        } else {
            ok = false;
        }
    }

    m_buffer->update();
    if (ok) {
        // update scroll bar
        if ((int)lineCount > m_xRange) {
            int lower = lineCount - m_xRange - 1;

            ui.horizontalScrollBar->setMaximum(lower);
            ui.horizontalScrollBar->setValue(lower);
            m_chart->axisX()->setRange(lower, lineCount - 1);
        } else {
            ui.horizontalScrollBar->setMaximum(0);
            m_chart->axisX()->setRange(0, m_xRange);
        }
    }

    file.close();
    return ok;
}

// d打开波形文件, 公有函数
void Oscilloscope::loadWave(const QString &fname)
{
    if (m_buffer->maximumCount() > 0) {
        QMessageBox::StandardButton button;

        button = QMessageBox::warning(this, tr("Warning"),
            tr("The current window has data not saved, "
               "Still open the file?"),
            QMessageBox::Yes | QMessageBox::Cancel);
        if (button != QMessageBox::Yes) {
            return;
        }
    }
    if (loadWave_p(fname) == false) {
        QMessageBox::critical(this, tr("Error"),
            tr("File parsing error."));
    }
}
