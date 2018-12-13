#ifndef OSCILLOSCOPEVIEW_H
#define OSCILLOSCOPEVIEW_H

#include "../abstractview.h"

namespace Ui {
class OscilloscopeView;
}
class OscopeTimeStamp;
class PointDataBuffer;
class ChannelItem;
class WaveDecode;
namespace QtCharts {
class QLineSeries;
class QChart;
}

class OscilloscopeView : public AbstractView {
    Q_OBJECT

public:
    OscilloscopeView(QWidget *parent = nullptr);
    ~OscilloscopeView();
    QString title() { return tr("Plot"); }
    QString iid() { return "oscilloscope"; }
    void retranslate();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void loadSettings(QSettings *config);
    void receiveData(const QByteArray &array);
    void setEnabled(bool enabled);
    void clear();
    bool holdReceive();

    QString openFileFilter();
    QString saveFileFilter();
    void saveFile(const QString &fileName, const QString &filter);
    void openFile(const QString &fileName, const QString &filter);

private:
    void setUseOpenGL(bool status);
    void setUseAntialiased(bool status);
    void setBackground(QColor color);
    void setGridColor(QColor color);
    void setUpdateInterval(int msec);

    void savePng(const QString &fileName);
    void saveBmp(const QString &fileName);
    void saveWave(const QString &fname);
    void loadWave(const QString &fname);

    void setupPlot();
    void listViewInit();
    bool loadWave_p(const QString &fname);
    QStringList csvSplitLine(const QString &line) {
        return line.split(", ");
    }
    ChannelItem* channelWidget(int channel);

private slots:
    void yOffsetChanged(double offset);
    void yRangeChanged(double range);
    void xRangeChanged(const QString &str);
    void channelStyleChanged(ChannelItem *item);
    void horzScrollBarMoved(int value);
    void horzScrollBarActionTriggered(void);
    void timeUpdata();

private:
    Ui::OscilloscopeView *ui;
    bool replotFlag = 1;
    QVector<QtCharts::QLineSeries*> m_series;
    QtCharts::QChart *m_chart;
    int m_count, m_xRange;
    QTimer *m_timer;
    OscopeTimeStamp *m_timeStamp;
    PointDataBuffer *m_buffer;
    WaveDecode *m_decode;
};

#endif // OSCILLOSCOPEVIEW_H
