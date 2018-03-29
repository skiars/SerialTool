#ifndef __WAVEDECODE_H
#define __WAVEDECODE_H

#include <QVector>

class QByteArray;

class WaveDecode {
public:
    enum DataMode {
        ValueMode,
        TimeStampMode
    };
    struct DataType {
        DataMode mode;
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint16_t msec;
        uint32_t sampleRate;
        double value;
        uint8_t channel;
    };

    WaveDecode();
    QVector<DataType> frameDecode(const QByteArray &array);

private:
    double data2Double(uint32_t value, int type);
    int pointData(DataType &dst, uint8_t byte);
    void timeStamp(DataType &dst, uint8_t* buffer);
    bool frameDecode_p(DataType &data, uint8_t byte);

private:
    uint32_t data;
    int type;
    char channel;
    int status;
    int frameCount, dataCount;
    int frameLength, dataLength;
    uint8_t infoFrame[8];
};

#endif // !__WAVEDECODE_H
