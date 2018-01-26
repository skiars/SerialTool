#ifndef DOCMENTDIALOG_H
#define DOCMENTDIALOG_H

#include <QDialog>

namespace Ui {
class DocmentDialog;
}

class DocmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocmentDialog(QWidget *parent = 0);
    ~DocmentDialog();

private:
    Ui::DocmentDialog *ui;
};

#endif // DOCMENTDIALOG_H
