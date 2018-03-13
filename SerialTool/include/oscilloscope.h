#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H

#include "ui_oscilloscope.h"

#include <QTimer>

#ifndef  CH_NUM
#define CH_NUM 16
#endif

struct WaveDataType;
class OscopeTimeStamp;
class PointDataBuffer;
class ChannelItem;
class QSettings;
namespace QtCharts {
class QLineSeries;
class QChart;
}

class Oscilloscope : public QWidget {
    Q_OBJECT

public:
    Oscilloscope(QWidget *parent = Q_NULLPTR);
    ~Oscilloscope();

    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);

    void start();
    void stop();
    bool holdReceive();

    void setUseOpenGL(bool status);
    void setUseAntialiased(bool status);
    void setBackground(QColor color);
    void setGridColor(QColor color);
    void setUpdateInterval(int msec);
    void addData(const WaveDataType& data);
    void clear();

    void savePng(const QString &fileName);
    void saveBmp(const QString &fileName);
    void saveWave(const QString &fname);
    void loadWave(const QString &fname);

private:
    void setupPlot();
    void listViewInit();
    bool loadWave_p(const QString &fname);
    QStringList csvSplitLine(const QString &line) {
        return line.split(", ");
    }
    ChannelItem* channelWidget(int channel) {
        return (ChannelItem *)(ui.channelList->itemWidget(ui.channelList->item(channel)));
    }

private slots:
    void yOffsetChanged(double offset);
    void yRangeChanged(double range);
    void xRangeChanged(const QString &str);
    void channelStyleChanged(ChannelItem *item);
    void horzScrollBarChanged(int value);
    void timeUpdata();

private:
    Ui_Oscilloscope ui;
    bool replotFlag = 1;
    QVector<QtCharts::QLineSeries*> m_series;
    QtCharts::QChart *m_chart;
    int m_count, m_xRange;
    QTimer updataTimer;
    OscopeTimeStamp* timeStamp;
    PointDataBuffer *m_buffer;
};

#endif
