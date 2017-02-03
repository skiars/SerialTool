#ifndef __OPTIONSBOX_H
#define __OPTIONSBOX_H

#include "ui_optionsbox.h"
#include <QtWidgets/QDialog>
#include "serialtool.h"

class OptionsBox : public QDialog {
    Q_OBJECT

public:
    OptionsBox(SerialTool *parent = Q_NULLPTR);
    ~OptionsBox();

private slots:
    void processOptions(QAbstractButton *button);
    void setTextFontAnsi();
    void setTextFontMulti();
    void setRxFontColor();
    void setTxFontColor();
    void setPlotBackgroundColor();
    void setAxisColor();

private:
    Ui_optionsBox ui;
    SerialTool *serialTool;
    QString fontFamily, fontStyle;
    int fontSize;
    QString rxColor, txColor, bgColor, axColor;
};

#endif
