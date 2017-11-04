#ifndef __WAVEDECODE_H
#define __WAVEDECODE_H

#include <stdint.h>

enum WaveDataMode {
    WaveValueMode,
    WaveTimeStampMode
};

struct WaveDataType {
    WaveDataMode mode;
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

class WaveDecode {
public:
    WaveDecode();
    bool frameDecode(WaveDataType &data, uint8_t byte);

private:
    double data2Double(uint32_t value, int type);
    int pointData(WaveDataType &dst, uint8_t byte);
    void timeStamp(WaveDataType &dst, uint8_t* buffer);

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
