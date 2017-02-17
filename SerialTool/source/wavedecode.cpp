#include "wavedecode.h"

enum {
    Frame_Head = 0xA3,      //帧头识别字
    Frame_PointMode = 0xA8, // 点模式识别字
    Frame_SyncMode = 0xA9   // 同步模式识别字
};

enum {
    STA_None = 0, // 空闲状态
    STA_Head,     // 接收到帧头
    STA_Point,    // 点模式
    STA_Sync,     // 同步模式
    STA_SyncData, // 同步模式数据
};

enum Result {
    Ok = 0,
    Error,
    Done,
};

static double data2Double(uint32_t value, int type)
{
    double d;
    union {
        uint32_t i;
        float f;
    } temp;

    switch (type) {
    case 0: // float
        temp.i = value;
        d = temp.f;
        break;
    case 1: // int8
        d = (int8_t)value;
        break;
    case 2: // int16
        d = (int16_t)value;
        break;
    case 3: // int32
        d = (int32_t)value;
        break;
    default:
        d = 0.0;
    }
    return d;
}

// 接收一个点数据, 仅仅是数据
static Result getPointData(char &ch, double &value, uint8_t byte)
{
    static char channel;
    static uint32_t data;
    static int count, len, type;
    const int bytes[] = { 4, 1, 2, 4 }; // 各种类型的字节数

    if (count == 0) { // 第一个字节是数据类型和通道信息
        channel = byte & 0x0F; // 通道值
        // tpye: 0: float, 1: int8, 2: int16, 3: int32
        type = byte >> 4;
        if (type > 3) { // 数据类型错误
            count = 0;
            return Error;
        }
        len = bytes[type];
    } else { // 后面几个字节是数据
        data = (data << 8) | byte;
        if (count >= len) { // 接收完毕
            ch = channel;
            value = data2Double(data, type);
            count = 0;
            data = 0;
            return Done;
        }
    }
    ++count;
    return Ok;
}

// 在帧中接收一个点, 会识别帧头
bool waveGetPointValue(char &ch, double &value, uint8_t byte)
{
    Result res;
    static int count, len, status = STA_None;

    // 捕获帧头状态机
    switch (status) {
    case STA_None:
        status = byte == Frame_Head ? STA_Head : STA_None;
        break;
    case STA_Head:
        /* byte == Frame_PointMode -> status = STA_Point
         * byte == Frame_SyncMode -> status = STA_Sync
         * else -> status = STA_None
         */
        status = byte == Frame_PointMode ? STA_Point
            : (byte == Frame_SyncMode ? STA_Sync : STA_None);
        break;
    case STA_Point:
        res = getPointData(ch, value, byte);
        switch (res) {
        case Ok: // 还在接收数据
            break;
        case Error: // 错误则重新开始接收
            status = STA_None;
            break;
        case Done: // 结束初始化状态并返回true
            status = STA_None;
            return true;
        }
        break;
    case STA_Sync:
        count = 0;
        len = byte;
        // 如果len > 64则帧长度错误, 将重新匹配帧, 否则转到STA_SyncData状态
        status = len <= 64 ? STA_SyncData : STA_None;
        break;
    case STA_SyncData:
        if (++count >= len) { // 计数达到帧长度说明帧结束, 重置状态
            status = STA_None;
        }
        res = getPointData(ch, value, byte);
        switch (res) {
        case Ok: // 还在接收数据
            break;
        case Error: // 错误则重新开始接收
            status = STA_None;
            break;
        case Done: // 结束返回true
            return true;
        }
        break;
    default: // 异常情况复位状态
        status = STA_None;
    }
    return false;
}
