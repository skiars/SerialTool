#include "docmentdialog.h"
#include "ui_docmentdialog.h"
#include <QFile>

DocmentDialog::DocmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocmentDialog)
{
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinMaxButtonsHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    ui->setupUi(this);
    QFile doc(":/doc/doc.html");
    doc.open(QFile::ReadOnly);
    ui->textBrowser->setHtml(doc.readAll());
    ui->textBrowser->setOpenExternalLinks(true); // 允许访问链接
    doc.close();
}

DocmentDialog::~DocmentDialog()
{
    delete ui;
}
