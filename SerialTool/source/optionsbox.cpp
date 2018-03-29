#include "optionsbox.h"
#include "ui_optionsbox.h"
#include <QFontDialog>
#include <QColorDialog>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>
#include <QSettings>
#include "mainwindow.h"

static QString languageName(const QString &path)
{
    QFile file(path + "/name");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray line = file.readLine();
        return QString(line).trimmed(); // 去除字符串两端的空白
    }
    return QString();
}

static void scanLanguagePath(const QString &path, QVector<QString> &languageList, QComboBox *comboBox)
{
    QDir dir(path);

    foreach(QFileInfo mfi , dir.entryInfoList()) {
        if (mfi.isDir() && mfi.fileName() != "." && mfi.fileName() != "..") {
            QString language;
            language = languageName(path + "/" + mfi.fileName());
            if (!language.isEmpty()) {
                comboBox->addItem(language);
                languageList.append(mfi.fileName());
            }
        }
    }
}

static void scanThemesPath(const QString &path, QComboBox *comboBox)
{
    QDir dir(path);

    foreach(QFileInfo mfi , dir.entryInfoList()) {
        if (mfi.isDir() && mfi.fileName() != "." && mfi.fileName() != "..") {
            QFile file(path + "/" + mfi.fileName() + "/style.css");
            if (file.exists()) {
                comboBox->addItem(mfi.fileName());
            }
        }
    }
}

OptionsBox::OptionsBox(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::OptionsBox)
{
    // 不显示问号
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    ui->setupUi(this);
    setFixedSize(400, 300); // 不能伸缩的对话框

    m_parent = parent;

    scanLanguagePath("language", languageList, ui->languageBox);
    scanThemesPath("themes", ui->themeBox);
    loadCommand();
    setup(); // 配置界面初始化

    connect(ui->fontAnsiSetButton, SIGNAL(clicked()), this, SLOT(setTextFontAnsi()));
    connect(ui->fontMultiSetButton, SIGNAL(clicked()), this, SLOT(setTextFontMulti()));
    connect(ui->rxColorButton, SIGNAL(clicked()), this, SLOT(setRxFontColor()));
    connect(ui->txColorButton, SIGNAL(clicked()), this, SLOT(setTxFontColor()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton *)),
        this, SLOT(processOptions(QAbstractButton *)));
    connect(ui->plotBgColorButton, SIGNAL(clicked()), this, SLOT(setPlotBackgroundColor()));
    connect(ui->axisColorButton, SIGNAL(clicked()), this, SLOT(setAxisColor()));
    connect(ui->cmdNew, SIGNAL(clicked()), this, SLOT(onCmdNewClick()));
    connect(ui->cmdEdit, SIGNAL(clicked()), this, SLOT(onCmdEditClick()));
    connect(ui->cmdDelete, SIGNAL(clicked()), this, SLOT(onCmdDeleteClick()));
    connect(ui->cmdList, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(setCmdItemColor()));
    connect(ui->languageBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLanguage(int)));
    connect(ui->useOpenGLBox, SIGNAL(clicked(bool)), this, SLOT(onUseOpenGLClick(bool)));
    connect(ui->opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(onOpacitySilderChanged(int)));
    connect(ui->opacitySpinBox, SIGNAL(editingFinished()), this, SLOT(onOpacitySpinBoxEdited()));
}

OptionsBox::~OptionsBox()
{
    delete ui;
}

// 初始化设置
void OptionsBox::setup()
{
    QSettings *config = m_parent->getConfig();

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
    ui->opacitySlider->setValue(config->value("WindowOpacity").toInt());
    ui->opacitySpinBox->setValue(ui->opacitySlider->value());
    ui->useOpenGLBox->setChecked(config->value("UseOpenGL").toBool());
    ui->useAntiBox->setChecked(config->value("UseAntialias").toBool());
    ui->updateIntervalBox->setValue(config->value("UpdateInterval").toInt());
    ui->portTypeBox->setCurrentIndex(config->value("PortType").toInt());
    language = config->value("Language").toString();
    theme = config->value("Theme").toString();
    config->endGroup();

    if (!fontFamily.isEmpty()) {
        ui->lineEditFontAnsi->setText(fontFamily.section(",", 0, 0).trimmed()
            + "," + fontStyle + "," + QString::number(fontSize));
        ui->lineEditFontMulti->setText(fontFamily.section(",", 1, 1).trimmed());
    }
    ui->lineEditRxColor->setText(rxColor);
    ui->lineEditTxColor->setText(txColor);
    ui->lineEditPlotColor->setText(bgColor);
    ui->lineEditAxisColor->setText(axColor);

    ui->languageBox->setCurrentText(languageName("language/" + language));
    ui->themeBox->setCurrentText(theme);

    // 使用OpenGL时不能使用抗锯齿
    if (ui->useOpenGLBox->isChecked()) {
        ui->useAntiBox->setEnabled(false);
    }
}

// 保存配置
void OptionsBox::processOptions(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton btn = ui->buttonBox->standardButton(button);

    if (btn == QDialogButtonBox::Ok || btn == QDialogButtonBox::Apply) {
        QSettings *config = m_parent->getConfig();
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
        config->setValue("WindowOpacity", QVariant(ui->opacitySpinBox->value()));
        config->setValue("UseOpenGL", QVariant(ui->useOpenGLBox->isChecked()));
        config->setValue("UseAntialias", QVariant(ui->useAntiBox->isChecked()));
        config->setValue("UpdateInterval", QVariant(ui->updateIntervalBox->value()));
        config->setValue("PortType", QVariant(ui->portTypeBox->currentIndex()));
        config->setValue("Language", QVariant(language));
        config->setValue("Theme", QVariant(ui->themeBox->currentText()));
        config->endGroup();
        saveCommand(); // 保存命令
        m_parent->loadSettings(); // 配置生效
        ui->retranslateUi(this);
        setup();    // 重新初始化OptionBox设置
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
        ui->lineEditFontAnsi->setText(font.family()
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
        ui->lineEditFontMulti->setText(font.family());
    }
}

void OptionsBox::setRxFontColor()
{
    QColor color = QColorDialog::getColor(QColor(rxColor), this);

    if (color.isValid()) {
        rxColor = color.name();
        ui->lineEditRxColor->setText(rxColor);
    }
}

void OptionsBox::setTxFontColor()
{
    QColor color = QColorDialog::getColor(QColor(txColor), this);

    if (color.isValid()) {
        txColor = color.name();
        ui->lineEditTxColor->setText(txColor);
    }
}

void OptionsBox::setPlotBackgroundColor()
{
    QColor color = QColorDialog::getColor(QColor(bgColor), this);

    if (color.isValid()) {
        bgColor = color.name();
        ui->lineEditPlotColor->setText(bgColor);
    }
}

void OptionsBox::setAxisColor()
{
    QColor color = QColorDialog::getColor(QColor(axColor), this);

    if (color.isValid()) {
        axColor = color.name();
        ui->lineEditAxisColor->setText(axColor);
    }
}

void OptionsBox::onCmdNewClick()
{
    QListWidgetItem *item = new QListWidgetItem();

    // 设置新建项为可以编辑的项目
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->cmdList->addItem(item);
    ui->cmdList->setCurrentItem(item);
    setCmdItemColor(); // 上色
}

void OptionsBox::onCmdEditClick()
{
    QListWidgetItem *item = ui->cmdList->currentItem();

    if (item) {
        ui->cmdList->editItem(item);
    }
}

void OptionsBox::onCmdDeleteClick()
{
    delete ui->cmdList->currentItem();
    setCmdItemColor(); // 上色
}

void OptionsBox::onUseOpenGLClick(bool status)
{
    ui->useAntiBox->setEnabled(!status);
}

void OptionsBox::setCmdItemColor()
{
    int i, count = ui->cmdList->count();
    QRegExp regExp("^[_A-Za-z][_0-9A-Za-z]{0,}$"); // 匹配正确的命令名格式

    for (i = 0; i < count; ++i) {
        QColor color;
        QListWidgetItem *item = ui->cmdList->item(i);
        QString str = item->text();
        if (!str.isEmpty() && (!regExp.exactMatch(str)
            || ui->cmdList->findItems(str, Qt::MatchExactly).count() > 1)) {
            color = QColor(0xFF7000);
        } else {
            color = i & 0x01 ? QColor(0xD6F3FF) : QColor(0xBDECFF);
        }
        item->setBackgroundColor(color);
    }
}

void OptionsBox::loadCommand()
{
    QFile file(QStandardPaths::writableLocation(
        QStandardPaths::AppConfigLocation) + "/keywords");

    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QTextStream text(&file);
        // 添加条目
        while (!text.atEnd()) {
            QString str;

            text >> str;
            // 不是空行就插入
            if (!str.isEmpty()) {
                QListWidgetItem *item = new QListWidgetItem(str);
                item->setFlags(item->flags() | Qt::ItemIsEditable);
                ui->cmdList->addItem(item);
            }
        }
        file.close();
        // 设置选中行
        if (ui->cmdList->count() > 0) {
            ui->cmdList->setCurrentRow(0);
            setCmdItemColor(); // 上色
        }
    }
}

void OptionsBox::onOpacitySilderChanged(int value)
{
    ui->opacitySpinBox->setValue(value);
}

void OptionsBox::onOpacitySpinBoxEdited()
{
    ui->opacitySlider->setValue(ui->opacitySpinBox->value());
}

void OptionsBox::saveCommand()
{
    QFile file(QStandardPaths::writableLocation
        (QStandardPaths::AppConfigLocation) + "/keywords");

    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        int i, count = ui->cmdList->count();

        QTextStream text(&file);
        for (i = 0; i < count; ++i) {
            QString str = ui->cmdList->item(i)->text();
            // 不是空行就插入
            if (!str.isEmpty()) {
                text << str << '\n'; // 写入文件
            }
        }
        file.close();
    }
}

void OptionsBox::setLanguage(int index)
{
    language = languageList[index];
}
