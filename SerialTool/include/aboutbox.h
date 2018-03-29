#ifndef __ABOUTBOX_H
#define __ABOUTBOX_H

#include <QtWidgets/QDialog>

namespace Ui {
    class AboutBox;
}

class AboutBox : public QDialog {
public:
        AboutBox(QWidget *parent);
        ~AboutBox();
private:
    Ui::AboutBox *ui;
};

#endif
