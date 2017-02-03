#include "serialtool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SerialTool w;
    w.show();

    return a.exec();
}
