#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H

#include "ui_oscilloscope.h"

#ifndef  CH_NUM
#define CH_NUM 16
#endif

struct WaveDataType;
class OscopeTimeStamp;
class ChannelItem;

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

    void setPlotAntialiased(bool status);
    void setGridAntialiased(bool status);
    void setBackground(QColor color);
    void setGridColor(QColor color);
    void setChannelColor(int channel, const QColor &color);
    void addData(const WaveDataType& data);
    void clear();

    void savePng(const QString &fileName);
    void saveBmp(const QString &fileName);
    void savePdf(const QString &fileName);
    void saveText(const QString &fname);

private:
    void setupPlot();
    void setupChannel();
    void listViewInit();
    uint64_t maxCount();
    void setChannelVisible(int channel, bool visible);

    ChannelItem* channelWidget(int channel) {
        return (ChannelItem *)(ui.channelList->itemWidget(ui.channelList->item(channel)));
    }

private slots:
    void yOffsetChanged(double offset);
    void yRangeChanged(double range);
    void xRangeChanged(const QString &str);
    void channelStyleChanged(ChannelItem *item);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);
    void horzScrollBarChanged(int value);
    void timeUpdata();

private:
    Ui_Oscilloscope ui;
    bool replotFlag = 1;
    uint64_t count[CH_NUM], _xRange;
    QTimer updataTimer;
    OscopeTimeStamp* timeStamp;
};

#endif
