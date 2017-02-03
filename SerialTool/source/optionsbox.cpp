#include "optionsbox.h"
#include <QFontDialog>
#include <QColorDialog>

OptionsBox::OptionsBox(SerialTool *parent) : QDialog(parent)
{
    // 不显示问号
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    ui.setupUi(this);
    setFixedSize(400, 300); // 不能伸缩的对话框

    serialTool = parent;
    
    QSettings *config = serialTool->getConfig();
    // 字体和颜色设置
    config->beginGroup("Settings");
    fontFamily = config->value("FontFamily").toString();
    fontFamily.replace("+", ",");
    fontStyle = config->value("FontStyle").toString();
    fontSize = config->value("FontSize").toInt();
    rxColor = config->value("ReceiveTextColor").toString();
    txColor = config->value("TransmitTextColor").toString();
    bgColor = config->value("PlotBackground").toString();
    axColor = config->value("AxisColor").toString();
    ui.plotAntiBox->setChecked(config->value("PlotAntialiased").toBool());
    ui.gridAntiBox->setChecked(config->value("GridAntialiased").toBool());
    config->endGroup();

    if (!fontFamily.isEmpty()) {
        ui.lineEditFontAnsi->setText(fontFamily.section(",", 0, 0).trimmed()
            + "," + fontStyle + "," + QString::number(fontSize));
        ui.lineEditFontMulti->setText(fontFamily.section(",", 1, 1).trimmed());
    }
    ui.lineEditRxColor->setText(rxColor);
    ui.lineEditTxColor->setText(txColor);
    ui.lineEditPlotColor->setText(bgColor);
    ui.lineEditAxisColor->setText(axColor);

    connect(ui.fontAnsiSetButton, SIGNAL(clicked()), this, SLOT(setTextFontAnsi()));
    connect(ui.fontMultiSetButton, SIGNAL(clicked()), this, SLOT(setTextFontMulti()));
    connect(ui.rxColorButton, SIGNAL(clicked()), this, SLOT(setRxFontColor()));
    connect(ui.txColorButton, SIGNAL(clicked()), this, SLOT(setTxFontColor()));
    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton *)),
        this, SLOT(processOptions(QAbstractButton *)));
    connect(ui.plotBgColorButton, SIGNAL(clicked()), this, SLOT(setPlotBackgroundColor()));
    connect(ui.axisColorButton, SIGNAL(clicked()), this, SLOT(setAxisColor()));
}

OptionsBox::~OptionsBox()
{

}

// 保存配置
void OptionsBox::processOptions(QAbstractButton *button)
{
    QString btnName = button->text();
    if (btnName == "OK" || btnName == "Apply") {
        QSettings *config = serialTool->getConfig();
        // 字体和颜色设置
        config->beginGroup("Settings");
        fontFamily.replace(",", "+");
        config->setValue("FontFamily", QVariant(fontFamily));
        fontFamily.replace("+", ",");
        config->setValue("FontStyle", QVariant(fontStyle));
        config->setValue("FontSize", QVariant(fontSize));
        config->setValue("ReceiveTextColor", QVariant(rxColor));
        config->setValue("TransmitTextColor", QVariant(txColor));
        config->setValue("PlotBackground", QVariant(bgColor));
        config->setValue("AxisColor", QVariant(axColor));
        config->setValue("PlotAntialiased", QVariant(ui.plotAntiBox->isChecked()));
        config->setValue("GridAntialiased", QVariant(ui.gridAntiBox->isChecked()));
        config->endGroup();
        serialTool->saveConfig(); // 先保存配置
        serialTool->loadConfig(); // 配置生效
    }
}

void OptionsBox::setTextFontAnsi()
{
    bool ok;
    QFont font = QFont(fontFamily.section(',', 0, 0), fontSize);
    font.setBold(fontStyle.indexOf("bold") != -1);
    font.setItalic(fontStyle.indexOf("italic") != -1);
    font = QFontDialog::getFont(&ok, font, this);
    if (ok) {
        fontFamily =
            font.family() + "," + fontFamily.section(',', 1, 1).trimmed();
        fontSize = font.pointSize();
        if (font.bold() && font.italic()) {
            fontStyle = "bold italic";
        } else if (font.bold()) {
            fontStyle = "bold";
        } else if (font.italic()) {
            fontStyle = "italic";
        } else {
            fontStyle = "normal";
        }
        ui.lineEditFontAnsi->setText(font.family()
            + "," + fontStyle + "," + QString::number(fontSize));
    }
}

void OptionsBox::setTextFontMulti()
{
    bool ok;
    QFont font = QFont(fontFamily.section(',', 1, 1), fontSize);
    font.setBold(fontStyle.indexOf("bold") != -1);
    font.setItalic(fontStyle.indexOf("italic") != -1);
    font = QFontDialog::getFont(&ok, font, this);
    if (ok) {
        fontFamily =
            fontFamily.section(',', 0, 0).trimmed() + "," + font.family();
        ui.lineEditFontMulti->setText(font.family());
    }
}

void OptionsBox::setRxFontColor()
{
    QColor color = QColorDialog::getColor(QColor(rxColor), this);
    rxColor = color.name();
    ui.lineEditRxColor->setText(rxColor);
}

void OptionsBox::setTxFontColor()
{
    QColor color = QColorDialog::getColor(QColor(txColor), this);
    txColor = color.name();
    ui.lineEditTxColor->setText(txColor);
}

void OptionsBox::setPlotBackgroundColor()
{
    QColor color = QColorDialog::getColor(QColor(bgColor), this);
    bgColor = color.name();
    ui.lineEditPlotColor->setText(bgColor);
}

void OptionsBox::setAxisColor()
{
    QColor color = QColorDialog::getColor(QColor(axColor), this);
    axColor = color.name();
    ui.lineEditAxisColor->setText(axColor);
}
