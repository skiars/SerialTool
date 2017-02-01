#ifndef __ABOUTBOX_H
#define __ABOUTBOX_H

#include "ui_aboutbox.h"
#include <QtWidgets/QDialog>

class AboutBox : public QDialog {
public:
        AboutBox(QWidget *parent);
private:
    Ui_AboutBox ui;
};

#endif
