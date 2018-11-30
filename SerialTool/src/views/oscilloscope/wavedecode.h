#ifndef WAVEDECODE_H
#define WAVEDECODE_H

#include <QVector>

class QByteArray;

class WaveDecode {
public:
    enum DataMode {
        ValueMode,
        TimeStampMode
    };
    struct TimeStamp {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint16_t msec;
        uint32_t sampleRate;
    };
    struct DataType {
        DataMode mode;
        uint8_t channel;
        union {
            TimeStamp ts;
            double value;
        } data;
    };

    WaveDecode();
    QVector<DataType> frameDecode(const QByteArray &array);

private:
    double data2Double(uint32_t value, int type);
    int pointData(DataType *data, uint8_t byte);
    void timeStamp(DataType *data, uint8_t* buffer);
    bool frameDecode_p(DataType *data, uint8_t byte);

private:
    uint32_t m_data;
    int m_type;
    char m_channel;
    int m_status;
    int m_frameCount, m_dataCount;
    int m_frameLength, m_dataLength;
    uint8_t m_infoFrame[8];
};

#endif // !__WAVEDECODE_H
