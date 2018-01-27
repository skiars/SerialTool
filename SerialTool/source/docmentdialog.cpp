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

    ui->textBrowser->setOpenExternalLinks(true); // 允许访问链接
    QFile doc("./doc/index.html");
    if (doc.open(QFile::ReadOnly)) {
        ui->textBrowser->setHtml(doc.readAll());
        doc.close();
    } else {
        ui->textBrowser->setHtml("<p>Cannot open the docments file.</p>\n"
                                 "<p><a href=\"https://github.com/Le-Seul/SerialTool"
                                 "/blob/master/SerialTool/doc/index.md\">View Docments"
                                 " On GitHub</a></p>");
    }
}

DocmentDialog::~DocmentDialog()
{
    delete ui;
}
