# SerialTool<br>

SerialTool是一个简单的串口调试工具，目前实现了文本收发和波形显示功能<br>

## 特点
* 使用Qt开发，轻松实现跨平台
* 中文接收显示不乱码
* 支持换肤（目前需要用户手动修改样式表文件）
* 波形显示默认最多支持16通道

## 更新计划
* 波形显示界面增加数值显示功能(显示各通道当前值)
* 串口调试助手界面增加语法高亮功能(通过正则表达式实现)
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
