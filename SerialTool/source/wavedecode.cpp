#include "wavedecode.h"

enum {
    Frame_Head = 0xA3,          //帧头识别字
    Frame_PointMode = 0xA8,     // 点模式识别字
    Frame_SyncMode = 0xA9,      // 同步模式识别字
    Frame_InfoMode  = 0xAA      // 信息帧识别字
};

enum {
    STA_None = 0, // 空闲状态
    STA_Head,     // 接收到帧头
    STA_Point,    // 点模式
    STA_Sync,     // 同步模式
    STA_Info,     // 信息模式
    STA_SyncData  // 同步模式数据
};

enum Result {
    Ok = 0,
    Error,
    Done,
};

WaveDecode::WaveDecode()
{
    status = STA_None;
    dataCount = 0;
    frameCount = 0;
    dataLength = 0;
    frameLength = 0;
}

double WaveDecode::data2Double(uint32_t value, int type)
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
int WaveDecode::pointData(DataType &dst, uint8_t byte)
{
    static const int bytes[] = { 4, 1, 2, 4 }; // 各种类型的字节数

    if (dataCount == 0) { // 第一个字节是数据类型和通道信息
        channel = byte & 0x0F; // 通道值
        // tpye: 0: float, 1: int8, 2: int16, 3: int32
        type = byte >> 4;
        if (type > 3) { // 数据类型错误
            dataCount = 0;
            return Error;
        }
        dataLength = bytes[type];
    } else { // 后面几个字节是数据
        data = (data << 8) | byte;
        if (dataCount >= dataLength) { // 接收完毕
            dst.channel = channel;
            dst.mode = ValueMode;
            dst.value = data2Double(data, type);
            dataCount = 0;
            data = 0;
            return Done;
        }
    }
    ++dataCount;
    return Ok;
}

// 转换时间戳
void WaveDecode::timeStamp(DataType &dst, uint8_t* buffer)
{
    dst.mode = TimeStampMode;
    dst.year = (buffer[0] >> 1) & 0x7F;
    dst.month = ((buffer[0] << 3) & 0x80) | ((buffer[1] >> 5) & 0x07);
    dst.day = buffer[1] & 0x1F;
    dst.hour = (buffer[2] >> 3) & 0x1F;
    dst.min = ((buffer[2] << 3) & 0x38) | ((buffer[3] >> 5) & 0x07);
    dst.sec = ((buffer[3] << 1) & 0x3E) | ((buffer[4] >> 7) & 0x01);
    dst.msec = (((uint16_t)buffer[4] << 3) & 0x03F8) | (((uint16_t)buffer[5] >> 5) & 0x0007);
    dst.sampleRate = (((uint32_t)buffer[5] << 16) & 0x1F0000)
        | (((uint32_t)buffer[6] << 8) & 0x00FF00) | (uint32_t)buffer[7];
}

// 波形数据帧解码, 会识别帧头
bool WaveDecode::frameDecode_p(DataType &data, uint8_t byte)
{
    int res;

    // 捕获帧头状态机
    switch (status) {
    case STA_None:
        status = byte == Frame_Head ? STA_Head : STA_None;
        break;
    case STA_Head:
        /* byte == Frame_PointMode -> status = STA_Point
         * byte == Frame_SyncMode -> status = STA_Sync
         * byte == Frame_InfoMode -> status = STA_Info
         * else -> status = STA_None
         */
        switch (byte) {
        case Frame_PointMode:
            status = STA_Point;
            break;
        case Frame_SyncMode:
            status = STA_Sync;
            break;
        case Frame_InfoMode:
            status = STA_Info;
            frameCount = 0;
            break;
        default:
            status = STA_None;
            break;
        }

        break;
    case STA_Point:
        res = pointData(data, byte);
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
        frameCount = 0;
        frameLength = byte;
        // 如果len > 64则帧长度错误, 将重新匹配帧, 否则转到STA_SyncData状态
        status = frameLength <= 64 ? STA_SyncData : STA_None;
        break;
    case STA_SyncData:
        if (++frameCount >= frameLength) { // 计数达到帧长度说明帧结束, 重置状态
            status = STA_None;
        }
        res = pointData(data, byte);
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
    case STA_Info:
        infoFrame[frameCount++] = byte;
        if (frameCount >= 8) {
            timeStamp(data, infoFrame);
            frameCount = 0;
            status = STA_None;
            return true;
        }
        break;
    default: // 异常情况复位状态
        status = STA_None;
    }
    return false;
}

QVector<WaveDecode::DataType> WaveDecode::frameDecode(const QByteArray &array)
{
    DataType data;
    QVector<DataType> vector;


    for (char byte : array) {
        if (frameDecode_p(data, byte) == true) {
            vector.append(data);
        }
    }
    return vector;
}
