#include "oscopetimestamp.h"
#include <QDate>
#include <QTime>

void OscopeTimeStamp::append(const WaveDecode::DataType &data, uint64_t count)
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

    m_timeStampVector.append(ts);
}


void OscopeTimeStamp::append(const QString &string, uint64_t count)
{
    TimeStamp_p ts;
    QStringList strList = string.split(' ');
    QDate date(QDate::fromString(strList[0], "yyyy-M-d"));
    QTime time(QTime::fromString(strList[1], "h:m:s.z"));

    ts.count = count;
    ts.year = (uint8_t)(date.year() - 2000);
    ts.month = (uint8_t)(date.month());
    ts.day = (uint8_t)(date.day());
    ts.hour = (uint8_t)(time.hour());
    ts.day = (uint8_t)(time.minute());
    ts.day = (uint8_t)(time.second());
    ts.day = (uint8_t)(time.msec());
    ts.sampleRate = (uint32_t)strList[2].replace("bps", "").toInt();

    m_timeStampVector.append(ts);
}

void OscopeTimeStamp::printTextStream(QTextStream &stream, uint64_t count)
{
    size_t i, size = m_timeStampVector.size();

    for (i = 0; i < size; ++i) {
        TimeStamp_p &ts = m_timeStampVector[i];

        if (ts.count == count) {
            char str[100];

            snprintf(str, sizeof(str), "# 20%.2d-%d-%d %.2d:%.2d:%.2d.%.3d %dbps",
                ts.year, ts.month, ts.day, ts.hour, ts.min, ts.sec, ts.msec, ts.sampleRate);
            stream << str << endl;
        }
    }
}
