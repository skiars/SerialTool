# 视频图传

视频图传的功能通过Tools -> Vedio Box菜单打开。该界面中，Save Image按钮会保存一张BMP格式的图片到当前的工作路径（该路径就是SerialTool保存文件的路径）中并自动命名，Copy C Array按钮会将该图片的C语言风格数组拷贝到剪切板中。

Vedio Box只能传输黑白图片，并且图像分辨率必须是80x60像素。

## 协议说明

下位机传输图像的协议非常简单，步骤如下：

1. 下位机发送字符`0x0B`。
2. 下位机发送字符`0xBB`。
3. 下位机发送图像数据，共600字节。

图像分辨率为80x60像素，图像数据的格式为：每个像素占用1bit，列优先编址，即每行80像素（10bytes），60行共600字节顺序发送。

## 代码示例

下面代码示范了下位机发送一副图像的过程：
``` C
char data[] = { 0x0b, 0xBB };

uart_senddata(data, 2);
uart_senddata((const char *)image_buff, 600);
```

其中`uart_senddata()`为串口发送函数，其声明如下：
``` C
void uart_senddata(const char *buffer, int size);
```

[Return to Index](./index.md)
