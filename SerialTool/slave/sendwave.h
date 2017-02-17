/*****************************************************************************
 * 文件名: sendwave.h
 *   版本: V1.0
 *   作者: 官文亮
 *   日期: 2017/2/12
 *   说明: 本文件属于SerialTool软件的波形显示功能的下位机参考代码, 作用是将数
 *         值转换为SerialTool可以识别的帧, 用户需实现串口发送函数, 结合本程序
 *         即可实现串口发送波形的显示, 本程序适合SerialTool v1.0.11及后续版本.
 *
 * SerialTool源码链接: https://github.com/Le-Seul/SerialTool
 * SerialTool安装包链接: https://github.com/Le-Seul/SerialTool/releases
 *
 *****************************************************************************/

#ifndef __SENDWAVE_H
#define __SENDWAVE_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
  extern "C" {
#endif
#endif // __cplusplus

/* 定义各通道 */
#define     CH1     0
#define     CH2     1
#define     CH3     2
#define     CH4     3
#define     CH5     4
#define     CH6     5
#define     CH7     6
#define     CH8     7
#define     CH9     8
#define     CH10    9
#define     CH11    10
#define     CH12    11
#define     CH13    12
#define     CH14    13
#define     CH15    14
#define     CH16    15

/* 公共函数声明 */
char ws_point_int8(char *buffer, char channel, int8_t value);
char ws_point_int16(char *buffer, char channel, int16_t value);
char ws_point_int32(char *buffer, char channel, int32_t value);
char ws_point_float(char *buffer, char channel, float value);
void ws_frame_init(char *buffer);
char ws_frame_length(const char *buffer);
char ws_add_int8(char *buffer, char channel, int8_t value);
char ws_add_int16(char *buffer, char channel, int16_t value);
char ws_add_int32(char *buffer, char channel, int32_t value);
char ws_add_float(char *buffer, char channel, float value);

#ifdef __cplusplus
#if __cplusplus
  }
#endif
#endif // __cplusplus

#endif // __SENDWAVE_H

/* end of file sendwave.h */
