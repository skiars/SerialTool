#include "oscilloscopeview.h"
#include "ui_oscilloscopeview.h"
#include <QTextStream>
#include <QSettings>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QLineEdit>
#include <QScreen>
#include <QMessageBox>
#include <QTimer>
#include "channelitem.h"
#include "oscopetimestamp.h"
#include "wavedecode.h"
#include "pointdatabuffer.h"

#define CH_NUM 16

QT_CHARTS_USE_NAMESPACE

OscilloscopeView::OscilloscopeView(QWidget *parent) :
    AbstractView(parent),
    ui(new Ui::OscilloscopeView),
    m_chart(nullptr),
    m_timer(new QTimer(this)),
    m_decode(new WaveDecode)
{
    m_chart = new QChart;
    ui->setupUi(this);

    m_timeStamp = new OscopeTimeStamp();
    m_xRange = 0;

    setupPlot();
    listViewInit();

    QRegExpValidator *pReg = new QRegExpValidator(QRegExp("^\\d{2,7}$"));
    ui->xRangeBox->lineEdit()->setValidator(pReg);

    m_timer->setInterval(25);

    connect(ui->horizontalScrollBar, &QAbstractSlider::sliderMoved, this, &OscilloscopeView::horzScrollBarMoved);
    connect(ui->horizontalScrollBar, &QAbstractSlider::actionTriggered, this, &OscilloscopeView::horzScrollBarActionTriggered);
    connect(ui->yOffsetBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &OscilloscopeView::yOffsetChanged);
    connect(ui->yRangeBox, static_cast<void (QDoubleSpinBox::*)(double)>
        (&QDoubleSpinBox::valueChanged), this, &OscilloscopeView::yRangeChanged);
    connect(ui->xRangeBox, &QComboBox::currentTextChanged, this, &OscilloscopeView::xRangeChanged);
    connect(m_timer, &QTimer::timeout, this, &OscilloscopeView::timeUpdata);

    clear();
}

OscilloscopeView::~OscilloscopeView()
{
    delete ui;
    delete m_timeStamp;
    delete m_chart;
}

// 重新设置语言
void OscilloscopeView::retranslate()
{
    ui->retranslateUi(this);
}

// load config
void OscilloscopeView::loadConfig(QSettings *config)
{
    config->beginGroup("Oscillograph");
    QString xRange = config->value("XRange").toString();
    double yRange = config->value("YRange").toDouble();
    double yOffset = config->value("YOffset").toDouble();
    ui->xRangeBox->setCurrentIndex(ui->xRangeBox->findText(xRange));
    ui->xRangeBox->setCurrentText(xRange);
    ui->yRangeBox->setValue(yRange);
    ui->yOffsetBox->setValue(yOffset);
    xRangeChanged(xRange);
    yRangeChanged(yRange);
    yOffsetChanged(yOffset);
    ui->holdReceiveBox->setChecked(config->value("HoldReceive").toBool());
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

// save config
void OscilloscopeView::saveConfig(QSettings *config)
{
    config->beginGroup("Oscillograph");
    config->setValue("YOffset", QVariant(ui->yOffsetBox->value()));
    config->setValue("YRange", QVariant(ui->yRangeBox->value()));
    config->setValue("XRange", QVariant(ui->xRangeBox->currentText()));
    config->setValue("HoldReceive", QVariant(ui->holdReceiveBox->isChecked()));
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

void OscilloscopeView::loadSettings(QSettings *config)
{
    setBackground(QColor(config->value("PlotBackground").toString()));
    setGridColor(QColor(config->value("AxisColor").toString()));
    // 绘制使用OpenGL加速
    setUseOpenGL(config->value("UseOpenGL").toBool());
    // 绘制时抗锯齿
    setUseAntialiased(config->value("UseAntialias").toBool());
    // 示波器刷新速度
    setUpdateInterval(config->value("UpdateInterval").toInt());
}

// 初始化示波器界面
void OscilloscopeView::setupPlot()
{
    ui->plotView->setChart(m_chart);
    m_chart->createDefaultAxes();
    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis;
    xAxis->setLabelFormat("%g");
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
void OscilloscopeView::listViewInit()
{
    ui->channelList->setModelColumn(2); // 两列
    for (int i = 0; i < CH_NUM; ++i) {
        QListWidgetItem *item = new QListWidgetItem;
        ui->channelList->addItem(item);
        ChannelItem *chItem = new ChannelItem("CH" + QString::number(i + 1));
        ui->channelList->setItemWidget(item, chItem);
        chItem->setChannel(i);
        channelStyleChanged(chItem);
        connect(chItem, &ChannelItem::changelChanged, this, &OscilloscopeView::channelStyleChanged);
    }
    ui->channelList->editItem(ui->channelList->item(0));
}

// 获取通道Widget
inline ChannelItem* OscilloscopeView::channelWidget(int channel)
{
    return (ChannelItem *)(ui->channelList->itemWidget(ui->channelList->item(channel)));
}

void OscilloscopeView::setEnabled(bool enabled)
{
    if (enabled) {
        m_timer->start();
    } else {
        m_timer->stop();
    }
}

// 返回保持接收状态
bool OscilloscopeView::holdReceive()
{
    return ui->holdReceiveBox->isChecked();
}

// 设置是否使用OpenGL加速
void OscilloscopeView::setUseOpenGL(bool status)
{
    for (int i = 0; i < CH_NUM; ++i) {
        m_series[i]->setUseOpenGL(status);
    }
    if (status) {
        setUseAntialiased(false);
    }
}

// 设置是否使用抗锯齿
void OscilloscopeView::setUseAntialiased(bool status)
{
    status &= !m_series[0]->useOpenGL(); // 不适用OpenGL时才可以打开抗锯齿
    ui->plotView->setRenderHint(QPainter::Antialiasing, status);
}

// 设置背景颜色
void OscilloscopeView::setBackground(QColor color)
{
    m_chart->setBackgroundPen(QPen(color));
    m_chart->setBackgroundBrush(QBrush(color));
}

// 设置网格和图例颜色
void OscilloscopeView::setGridColor(QColor color)
{
    m_chart->axisX()->setLinePen(QPen(color));
    m_chart->axisX()->setGridLineColor(color);
    m_chart->axisX()->setLabelsColor(color);
    m_chart->axisX()->setMinorGridLinePen(QPen(color, 0.35));
    m_chart->axisY()->setLinePen(QPen(color));
    m_chart->axisY()->setGridLineColor(color);
    m_chart->axisY()->setLabelsColor(color);
    m_chart->axisY()->setMinorGridLinePen(QPen(color, 0.35));
}

// 设置更新时间
void OscilloscopeView::setUpdateInterval(int msec)
{
    m_timer->setInterval(msec);
}

// 添加数据
void OscilloscopeView::receiveData(const QByteArray &array)
{
    QVector<WaveDecode::DataType> vector = m_decode->frameDecode(array);

    for (WaveDecode::DataType data : vector) {
        if (data.mode == WaveDecode::ValueMode) {
            m_buffer->append(data.channel, data.data.value); // 先将数据暂存到缓冲区
        } else { // Wave Time StampMode
            m_timeStamp->append(data, m_buffer->maximumCount());
        }
    }
}

// 清空数据
void OscilloscopeView::clear()
{
    m_buffer->clear();
    m_timeStamp->clear();
    ui->horizontalScrollBar->setMaximum(0);
    m_chart->axisX()->setRange(0, m_xRange);
}

// 保存PNG文件
void OscilloscopeView::savePng(const QString &fileName)
{
    QScreen *screen = QGuiApplication::primaryScreen();

    QPixmap p = screen->grabWindow(ui->plotView->winId());
    QImage image = p.toImage();
    image.save(fileName);
}

// 保存BMP文件
void OscilloscopeView::saveBmp(const QString &fileName)
{
    QScreen *screen = QGuiApplication::primaryScreen();

    QPixmap p = screen->grabWindow(ui->plotView->winId());
    QImage image = p.toImage();
    image.save(fileName);
}

// 通道显示风格改变
void OscilloscopeView::channelStyleChanged(ChannelItem *item)
{
    int ch = item->channel();
    m_series[ch]->setVisible(item->isChecked());
    m_series[ch]->setPen(QPen(item->color(), 1.1));
}

// 滚动条滑块移动时触发
void OscilloscopeView::horzScrollBarMoved(int value)
{
    if (ui->horizontalScrollBar->maximum() == value) {
        replotFlag = true;
    } else {
        replotFlag = false;
    }
    m_chart->axisX()->setRange(value, value + m_xRange);
}

// 滚动条按钮点击时触发
void OscilloscopeView::horzScrollBarActionTriggered(void)
{
    horzScrollBarMoved(ui->horizontalScrollBar->value());
}

// Y轴偏置改变
void OscilloscopeView::yOffsetChanged(double offset)
{
    double range = ui->yRangeBox->value();

    m_chart->axisY()->setRange(offset - range * 0.5, offset + range * 0.5);
}

// Y轴范围改变
void OscilloscopeView::yRangeChanged(double range)
{
    double offset = ui->yOffsetBox->value();

    m_chart->axisY()->setRange(offset - range * 0.5, offset + range * 0.5);
}

// X轴范围改变
void OscilloscopeView::xRangeChanged(const QString &str)
{
    m_count = m_buffer->update();
    m_xRange = str.toDouble();
    ui->horizontalScrollBar->setPageStep(m_xRange);

    int count = m_count - 1;
    if (count > m_xRange) {
        int lower = count - m_xRange;
        m_chart->axisX()->setRange(lower, count);
        bool req = ui->horizontalScrollBar->value()
                == ui->horizontalScrollBar->maximum();
        ui->horizontalScrollBar->setMaximum(lower);
        if (req) {
            ui->horizontalScrollBar->setValue(lower);
        }
    } else {
        m_chart->axisX()->setRange(0, m_xRange);
        ui->horizontalScrollBar->setMaximum(0);
    }
}

// 更新定时器触发
void OscilloscopeView::timeUpdata()
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
        bool req = ui->horizontalScrollBar->value()
                == ui->horizontalScrollBar->maximum();
        ui->horizontalScrollBar->setMaximum(lower);
        if (req) {
            ui->horizontalScrollBar->setValue(lower);
        }
    }
    m_count = count;
}

// 保存txt文件
void OscilloscopeView::saveWave(const QString &fname)
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
        m_timeStamp->printTextStream(out, i);
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
bool OscilloscopeView::loadWave_p(const QString &fname)
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
            m_timeStamp->append(lineList[0].mid(2), lineCount);
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

            ui->horizontalScrollBar->setMaximum(lower);
            ui->horizontalScrollBar->setValue(lower);
            m_chart->axisX()->setRange(lower, lineCount - 1);
        } else {
            ui->horizontalScrollBar->setMaximum(0);
            m_chart->axisX()->setRange(0, m_xRange);
        }
    }

    file.close();
    return ok;
}

// 打开波形文件, 公有函数
void OscilloscopeView::loadWave(const QString &fname)
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

QString OscilloscopeView::openFileFilter()
{
    return tr("Wave Plain Text File (*.txt)");
}

QString OscilloscopeView::saveFileFilter()
{
    return tr("Portable Network Graphic Format (*.png)") + ";;" +
           tr("Bitmap (*.bmp)") + ";;" +
           tr("Wave Plain Text File (*.txt)");
}

void OscilloscopeView::saveFile(const QString &fileName, const QString &filter)
{
    if (filter == tr("Portable Network Graphic Format (*.png)")) {
        savePng(fileName);
    } else if (filter == tr("Bitmap (*.bmp)")) {
        saveBmp(fileName);
    } else if (filter == tr("Wave Plain Text File (*.txt)")) {
        saveWave(fileName);
    }
}

void OscilloscopeView::openFile(const QString &fileName, const QString &filter)
{
    if (filter == tr("Wave Plain Text File (*.txt)")) {
        loadWave(fileName);
    }
}
