

# SerialTool

SerialTool是一个实用的串口调试工具，这款工具支持串口调试助手、波形显示和文件传输等功能。该工具软件使用GPL许可证发布。

## 特点
* 使用Qt开发，轻松实现跨平台
* 中文接收显示不乱码
* 支持换肤
* 波形显示默认最多支持16通道
* 支持多国语言
* 串口调试助手支持语法高亮(bash模式)
* 支持TCP/UDP和串口两种端口模式
* 支持文件传输(目前只支持XModem协议)

## 下载地址
* [GitHub](https://github.com/Le-Seul/SerialTool/releases)
* [百度网盘](http://pan.baidu.com/s/1c18ZXW8)

## 更新计划
* 波形显示界面增加数值显示功能(显示各通道当前值)
* 其他...

## 项目信息

* 下位机示例代码在[.\SerialTool\slave](https://github.com/Le-Seul/SerialTool/tree/master/SerialTool/slave)目录下，该目录有两个文件：
 * [sendwave.c](https://github.com/Le-Seul/SerialTool/blob/master/SerialTool/slave/sendwave.c)
 * [sendwave.h](https://github.com/Le-Seul/SerialTool/blob/master/SerialTool/slave/sendwave.h)
* 本项目使用的Qt库为Qt 5.6.1 for Visual Studio 2015和Qt 5.6.2 for MinGW 
* 使用的插件:
 * QScintilla: [Documentation](http://pyqt.sourceforge.net/Docs/QScintilla2), [Download](https://riverbankcomputing.com/software/qscintilla/download)
 * [QCustomPlot](http://www.qcustomplot.com/): 源码已经包含到项目中
* 其他信息参看[readme.txt](https://github.com/Le-Seul/SerialTool/blob/master/readme.txt)文件

## 文档

[SerialTool使用说明](./SerialTool/doc/index.md)
