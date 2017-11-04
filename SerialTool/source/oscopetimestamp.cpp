#include "oscopetimestamp.h"
#include "wavedecode.h"

void OscopeTimeStamp::append(const WaveDataType &data, uint64_t count)
{
    TimeStamp_p ts;

    ts.count = count;
    ts.year = data.year;
    ts.month = data.month;
    ts.day = data.day;
    ts.hour = data.hour;
    ts.min = data.min;
    ts.sec = data.sec;
    ts.msec = data.msec;
    ts.sampleRate = data.sampleRate;

    timeStamp.append(ts);
}

void OscopeTimeStamp::printTextStream(QTextStream &stream, uint64_t count)
{
    size_t i, size = timeStamp.size();

    for (i = 0; i < size; ++i) {
        TimeStamp_p &ts = timeStamp[i];

        if (ts.count == count) {
            char str[100];

            snprintf(str, sizeof(str), "# 20%.2d-%d-%d %.2d:%.2d:%.2d.%.3d %dbps",
                ts.year, ts.month, ts.day, ts.hour, ts.min, ts.sec, ts.msec, ts.sampleRate);
            stream << str << endl;
        }
    }
}
