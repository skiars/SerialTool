# 串口示波器协议

## 概述

为了使用串口示波器，用户需要按照SerialTool软件的协议发送数据，我们提供了sendwave.c以及sendwave.h这两个文件来封装这些协议，这两个文件在源码目录的[slave](https://github.com/Le-Seul/SerialTool/tree/master/SerialTool/slave)文件夹下。用户如果需要修改或者扩充协议可自行阅读源码。

串口示波器分为两种传输模式：点发送模式和同步发送模式。点发送模式每个点都是独立发送的，而同步发送模式是将所有待发送的通道数据一起发送。因此，相比同步发送模式，点发送模式各通道之间的时间轴可能存在一些偏差，一般在±1个单位时间之间。但是点发送模式使用起来可能更为灵活。

SerialTool还支持时间戳的发送，以方便数据分析。

## 点发送模式

点发送模式由以下函数来实现：

``` C
char ws_point_int8(char *buffer, char channel, int8_t value);
char ws_point_int16(char *buffer, char channel, int16_t value);
char ws_point_int32(char *buffer, char channel, int32_t value);
char ws_point_float(char *buffer, char channel, float value);
```

这些函数的参数说明：

* `buffer`:

  > 工作缓冲区，它最多可能需要7bytes空间。

* `channle`:

  > 发送的通道，取值为CH1 ~ CH16。

* `value`:

  > 要发送的数值，其类型由使用的函数决定（包括`int8_t`, `int16_t`, 
    `int32_t`或者`float`）。

这些函数在sendwave.c中有详细的说明。以`ws_point_int16()`为例，调用方式如下：
``` C
char buffer[7], len;
int16_t value = 100; // a test value

len = ws_point_int16(buffer, CH1, value);
sendBuffer(buffer, len); // serial port send data
```

## 同步发送模式

同步发送模式由以下函数实现：
``` C
void ws_frame_init(char *buffer);
char ws_frame_length(const char *buffer);
char ws_add_int8(char *buffer, char channel, int8_t value);
char ws_add_int16(char *buffer, char channel, int16_t value);
char ws_add_int32(char *buffer, char channel, int32_t value);
char ws_add_float(char *buffer, char channel, float value);
```

首先要调用`ws_frame_init()`函数来初始化缓冲区。调用`ws_frame_length()`可以获取缓冲区占用的字节数。

`ws_add_xxx()`函数用于向缓冲区中添加一个数据点，参数列表如下：

* `buffer`:

  > 已经初始化的帧缓冲区。

* `channle`:

  > 发送的通道，取值为CH1 ~ CH16。

* `value`:

  > 要发送的数值，其类型由使用的函数决定（包括`int8_t`, `int16_t`, 
    `int32_t`或者`float`）。

这些函数如果返回1就表示数据点添加成功，0表示添加失败（缓冲区达到最大长度）。

同步发送模式的例程如下：
``` C
char buffer[83]; // maximum use of 83 bytes
int value = 10000; // a test value

ws_frame_init(buffer);
ws_add_int8(buffer, CH1, (int8_t)(value / 100));
ws_add_int16(buffer, CH2, (int16_t)(value / 20));
ws_add_float(buffer, CH3, (float)value * 0.1f);
sendBuffer(buffer, ws_frame_length(buffer)); // serial port send data
```

使用同步模式发送时，可以在同一帧中使用不同的数据类型，但是不建议使用重复的通道，因为这样可能会使缓冲区很快用完并导致时间轴错乱。

## 时间戳

利用时间戳，可以将下位机的时间信息发送至上位机。上位机收到时间戳并不会显示，但是在存储波形文件时会记录时间戳，因此时间戳可以方便地进行数据分析。

下位机要发送时间戳，需要使用下列函数：
``` C
char ws_send_timestamp(char *buffer, ws_timestamp_t* ts);
```

`ws_timestamp_t`结构体的定义如下：
``` C
typedef struct {
    uint8_t year;           // 0~99 -> 2000 ~ 2099, 7 bit
    uint8_t month;          // month, 1 ~ 12
    uint8_t day;            // day, 1 ~ 31
    uint8_t hour;           // hour, 0 ~ 23
    uint8_t min;            // minute, 0 ~ 59
    uint8_t sec;            // second, 0 ~ 59
    uint16_t msec;          // millisecond, 0 ~ 999
    uint32_t sampleRate;    // sample rate, 0 ~ 2000000
} ws_timestamp_t;
```

发送时间戳的例程如下：
``` C
char buffer[10], len;

len = ws_send_timestamp(buffer, &ts);
sendBuffer(buffer, len); // serial port send data
```
下位机通常不需要重复发送时间戳，因为这个时间信息一般只用于记录或者辨识，相比而言，采样率可能在数据分析时会更有用。因此我建议下位机在以下时机发送一次时间戳：

* 下位机运行之初。
* 数据采样率发生变化之后。
* 中断波形传输后准备再次发送波形数据前。

## 完整示例代码

``` C
// include file
#include <math.h>
#include "sendwave.h"

// 串口示波器测试函数
void plotTest(void)
{
    char buffer[100], len; // 缓冲区最大需要83bytes
    static float key = 0.0f;
    static ws_timestamp_t ts = { // 时间戳
        17, 11, 20,      // year, month, day
        12, 30, 45,      // hour, minute, second
        120, 50          // msec, sample rate
    };

    // 每秒钟发送一次时间戳
    if ((int64_t)(key * 50.0) % 100 == 0) {
        len = ws_send_timestamp(buffer, &ts);
        sendBuffer(buffer, len); // 串口发送数据
    }

    // 点发送模式 int8类型（8位有符号整形）
    len = ws_point_int8(buffer, CH1, (int8_t)(sinf(key) * 64));
    sendBuffer(buffer, len); // 串口发送数据

    // 点发送模式 int16类型（16位有符号整形）
    len = ws_point_int16(buffer, CH2, (int16_t)(sinf(key) * 4096));
    sendBuffer(buffer, len); // 串口发送数据

    // 点发送模式 int32类型（32位有符号整形）
    len = ws_point_int32(buffer, CH3, (int32_t)(sinf(key) * 2048));
    sendBuffer(buffer, len); // 串口发送数据

    // 点发送模式 float类型（单精度浮点型）
    len = ws_point_float(buffer, CH4, (float)(sinf(key) * 512));
    sendBuffer(buffer, len); // 串口发送数据

    // 同步模式(帧发送模式)
    ws_frame_init(buffer);
    ws_add_int8(buffer, CH5, (int8_t)(sinf(key) * 128));
    ws_add_int16(buffer, CH6, (int16_t)(sinf(key) * 700));
    ws_add_int32(buffer, CH7, (int32_t)(sinf(key) * 1024));
    ws_add_float(buffer, CH9, (float)(sinf(key) * 256));
    sendBuffer(buffer, ws_frame_length(buffer)); // 串口发送数据

    // 更新时间计数
    key += 0.02f;
}
```

## 移植

sendwave.c和sendwave.h中的代码在gcc和KEIL for ARM中可以编译通过，但是我们不能保证它可以在所有编译器下编译成功。这些代码需要标准库中包含stdint.h文件，如果没有这个文件，您可以使用以下代码来替换:
``` C
// #include <stdint.h>

typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed long  int32_t;
```

用户一般不会使用到所有的API，例如您可能不需要使用`ws_point_float()`以及`ws_add_float()`函数。通常，编译器不会链接未用到的函数。编译器如果会链接从未被调用的函数，并且要保证目标代码最少，您应当删除这些函数的源码。

如果您在移植串口示波器的协议时遇到了任何问题，您也可以向作者咨询。

[Return to Index](./index.md)
