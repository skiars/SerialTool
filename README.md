

# SerialTool

SerialTool是一个实用的串口调试工具，这款工具支持串口调试助手、波形显示和文件传输等功能。该工具软件使用GPL许可证发布。用户可以将波形文件保存为文本文件，然后使用Matlab等工具进行数据分析。如果您支持本软件，欢迎贡献源代码或者向作者提出建议。

## 特点
* 使用Qt开发，轻松实现跨平台
* 中文接收显示不乱码
* 支持换肤
* 波形显示默认最多支持16通道，也可以重新编译支持更多通道
* 支持波形数据保存（纯文本，csv格式）
* 支持波形数据读取
* 支持时间戳功能，包括年、月、日、时、分、秒、毫秒、采样率，方便进行波形数据分析（时间戳由下位机发送）
* 多语言支持
* 串口调试助手支持语法高亮(bash模式)
* 支持TCP/UDP和串口两种端口模式
* 支持文件传输(目前只支持XModem协议)

## 下载地址
* [GitHub](https://github.com/Le-Seul/SerialTool/releases)
* [百度网盘](http://pan.baidu.com/s/1c18ZXW8)

## 项目信息

* 下位机示例代码在[./SerialTool/slave](./SerialTool/slave)目录下，该目录有两个文件：
 * [sendwave.c](./SerialTool/slave/sendwave.c)
 * [sendwave.h](./SerialTool/slave/sendwave.h)
 您可以参考[串口示波器协议说明](./SerialTool/doc/plot_protocol.md)来了解下位机该如何发送波形数据。
* 跟随本项目发布的可执行文件安装包使用Qt 5.6.3 for MinGW编译。
* 使用的插件:
 * QScintilla: [Documentation](http://pyqt.sourceforge.net/Docs/QScintilla2), [Download](https://riverbankcomputing.com/software/qscintilla/download)
 * Qt Charts: 此插件在Qt 5.7以及更高的版本中包含在Qt安装文件中，使用Qt 5.6时需要自行编译。

## 开源协议

本程序使用GPL协议发布，但是[./SerialTool/slave](./SerialTool/slave)目录下的源码不受GPL协议约束，用户可以随意使用。

[License](./LICENSE)

## 文档

[SerialTool使用说明](./SerialTool/doc/index.md)
