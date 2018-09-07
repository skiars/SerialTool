#include "aboutbox.h"
#include "ui_aboutbox.h"
#include <QFile>
#include "version.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    // 不显示问号
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    setFixedSize(400, 400); // 不能伸缩的对话框

    QPixmap pix(":/SerialTool/images/logo.ico");
    pix = pix.scaledToWidth(64, Qt::SmoothTransformation);
    ui->label1->setPixmap(pix);
    ui->label2->setText(
        "<b>SerialTool</b><br>"
        "Version: " SOFTWARE_VERSION " (" BUILD_VERSION ")<br>"
        "Email: 2269610337@qq.com<br>"
        "Home Page: <a href=\"https://github.com/gztss/SerialTool\">View Code On GitHub</a><br>"
        "Build Date: " __DATE__ "<br>" // 编译时间
        COPYRIGHT
    );
    ui->label2->setOpenExternalLinks(true); // 允许访问链接
    QFile lic(":/license/license.html");
    lic.open(QFile::ReadOnly);
    ui->textBrowser->setHtml(lic.readAll());
    ui->textBrowser->setOpenExternalLinks(true); // 允许访问链接
    lic.close();
}

AboutBox::~AboutBox()
{
    delete ui;
}
