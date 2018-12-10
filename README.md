
<p align="center">
  <a>
    <img src="https://raw.githubusercontent.com/wiki/gztss/SerialTool/image/logo.png" alt="logo" width=64 height=64>
  </a>
  <h2 align="center">SerialTool</h2>
  <p align="center">
    <a href="https://github.com/gztss/SerialTool/blob/master/LICENSE">
      <img src="https://img.shields.io/github/license/gztss/SerialTool.svg" alt="license" />
    </a>
    <a href="https://github.com/gztss/SerialTool/issues">
      <img src="https://img.shields.io/github/issues/gztss/SerialTool.svg" alt="issues">
    </a>
    <a>
      <img src="https://img.shields.io/github/repo-size/gztss/SerialTool.svg" alt="Repo Size" />
    </a>
    <a href="https://github.com/gztss/SerialTool/archive/master.zip">
      <img src="https://img.shields.io/github/languages/code-size/gztss/SerialTool.svg" alt="Code Size" />
    </a>
    <a href="https://github.com/gztss/SerialTool/releases">
      <img src="https://img.shields.io/github/downloads/gztss/SerialTool/total.svg" alt="All Downloads" />
    </a>
    <a href="https://github.com/gztss/SerialTool/releases">
      <img src="https://img.shields.io/github/release/gztss/SerialTool/all.svg" alt="Last Release">
    </a>
  </p>
  <p align="center">A practical Serial-Port/TCP/UDP debugging tool.</p>
</p>

## 简介

SerialTool是一个跨平台的串口/网络调试工具。此工具支持串口调试助手、终端、波形显示和文件传输等功能。该工具的源代码使用GPL许可证发布。

SerialTool支持基础的文本传输。文本传输界面支持16进制与ASCII格式的收发。您可以自定义文本传输使用的字符编码，目前SerialTool支持ASCII、GB-2312、GB-18030、UTF-8、UTF-16BE、UTF-16LE共6总字符编码。此外还支持多种代码高亮模式，这对文本通信协议（例如使用JSON通信）的操作有较大帮助。

SerialTool的波形显示模块使用简单的协议进行数据传输，我们已经为用户封装好了平台无关的[C语言协议库](./SerialTool/slave)，具体的协议说明请参考[串口示波器协议说明](../../plot_protocol.md)。用户可以将采集的波形保存为文本文件，然后使用Matlab等工具进行数据分析（您需要自己编写程序，SerialTool的波形存档为csv格式）。当然，您也可以使用SerialTool载入波形文件。

如果您支持本软件，欢迎贡献源代码或者向作者提出建议。

## 特性
* 使用Qt开发，跨平台。目前支持Windows和Linux
* 中文接收显示不乱码
* 国际化支持
* 换肤
* 波形显示默认最多支持16通道，也可以重新编译支持更多通道
* 波形数据保存（纯文本，csv格式）与读取
* 波形数据支持时间戳。时间戳包括年、月、日、时、分、秒、毫秒、采样率，方便进行波形数据分析（时间戳由下位机发送）
* 文本收发界面支持语法高亮(Bash, JSON, Lua, C/C++等)
* 文本收发界面支持多种字符编码，如GB2312, UTF8, UTF16等
* 通信接口支持TCP/UDP和串口
* 支持文件传输(目前只支持XModem协议)

## [查看Wiki](../../wiki)

## 下载地址
* [Latest release](https://github.com/gztss/SerialTool/releases/latest)
* [GitHub](https://github.com/Le-Seul/SerialTool/releases)
* [百度网盘](http://pan.baidu.com/s/1c18ZXW8) (不推荐😂😂)

## 项目信息

* 下位机示例代码在[./SerialTool/slave](./SerialTool/slave)目录下，该目录有两个文件：
  * [sendwave.c](./SerialTool/slave/sendwave.c)
  * [sendwave.h](./SerialTool/slave/sendwave.h)
  * 您可以参考[串口示波器协议说明](../../plot_protocol.md)来了解下位机该如何发送波形数据。
* 跟随本项目发布的Windows 32位安装包使用Qt 5.6.3 for MinGW编译。
* 使用的插件:
  * QScintilla: [Documentation](http://pyqt.sourceforge.net/Docs/QScintilla2), [Download](https://riverbankcomputing.com/software/qscintilla/download)
  * Qt Charts: 此插件在Qt 5.7以及更高的版本中包含在Qt安装文件中，使用Qt 5.6时需要自行编译。

## 开源协议

本程序遵从[GPL-3.0协议](./LICENSE)发布，[./SerialTool/slave](./SerialTool/slave)目录下的源码不受GPL-3.0协议约束，用户可以将这些代码加入到自己的项目中而不必公开。

[License](./LICENSE)
