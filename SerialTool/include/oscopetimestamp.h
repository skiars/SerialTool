#ifndef __OSCOPE_TIME_STAMP_H
#define __OSCOPE_TIME_STAMP_H


#include <QVector>
#include <QTextStream>

struct WaveDataType;

class OscopeTimeStamp {
public:
    OscopeTimeStamp() { timeStamp.clear(); }
    void printTextStream(QTextStream &stream, uint64_t count);
    void append(const WaveDataType &data, uint64_t count);
    void append(const QString &string, uint64_t count);
    void clear() { timeStamp.clear(); }

private:
    struct TimeStamp_p {
        uint64_t count;
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint16_t msec;
        uint32_t sampleRate;
    };

    QVector<TimeStamp_p> timeStamp;
};

#endif
