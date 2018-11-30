#ifndef ABOUTBOX_H
#define ABOUTBOX_H

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

#endif // ABOUTBOX_H
