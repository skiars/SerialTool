#include "settingswidgets.h"
#include "translate.h"
#include <QSettings>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDir>
#include <QFileInfo>
#include <QComboBox>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFontDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QListView>
#include <QDebug>

AbstractSettingsWidget::AbstractSettingsWidget(
        const QJsonObject &json,
        const QString &path,
        QWidget *parent)
      : QWidget(parent)
{
    m_layout = createLayout();
    m_layout->setMargin(0);
    this->setLayout(m_layout);
    m_key = json.value("key").toString();
    m_id = json.value("id").toString();
    m_path = path;

    QJsonValue value = json.value("mutex");
    if (value.isObject()) {
        QJsonObject object = value.toObject();

        m_mutexActiveStatus = object.value("active").toVariant();
        QJsonArray array = object.value("list").toArray();
        for (QJsonValue v : array) {
            QString s = v.toString();
            if (!s.isEmpty()) {
                m_mutexList.append(s);
            }
        }
        m_mutexList.sort();
    }
}

void AbstractSettingsWidget::setMutexStatus(const QVariant &status)
{
    if (m_mutexActiveStatus.isValid() && !m_mutexList.isEmpty()) {
        emit mutexChanged(status != m_mutexActiveStatus, m_mutexList);
    }
}

void AbstractSettingsWidget::setEnabled(bool enabled, const void *widget)
{
    if (widget != nullptr) {
        uint32_t wid = (uint32_t)reinterpret_cast<size_t>(widget);
        if (enabled) {
            m_disableList.removeOne(wid);
        } else {
            if (!m_disableList.contains(wid)) {
                m_disableList.append(wid);
            }
        }
        bool status = m_disableList.isEmpty();
        QWidget::setEnabled(status);
    }
}

SettingsComboBox::SettingsComboBox(const QJsonObject &json,
                                   const QString &path,
                                   QWidget *parent)
                                 : AbstractSettingsWidget(json, path, parent)
{
    QJsonValue value = json.value("items");

    m_comboBox = new QComboBox(this);
    m_comboBox->setView(new QListView());
    m_layout->addWidget(m_comboBox);
    m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    if (value.isArray()) {
        QJsonArray array = value.toArray();
        for (QJsonValue v : array) {
            if (v.isString()) {
                m_comboBox->addItem(v.toString());
                m_items.append(v.toString());
            }
        }
    } else if (value.isString()) {
        parseItems(value.toString());
    }
}

void SettingsComboBox::loadSettings(QSettings *config)
{
    m_comboBox->setCurrentIndex(m_items.indexOf(config->value(m_key).toString()));
}

void SettingsComboBox::saveSettings(QSettings *config)
{
    int index = m_comboBox->currentIndex();
    if (index >= 0) {
        config->setValue(m_key, m_items[index]);
    }
}

void SettingsComboBox::parseItems(const QString &str)
{
    QStringList list = str.split('*');
    QString path = list[0];
    QString mode = list.size() == 2 ? list[1] : "";
    QDir dir(path);

    for (QFileInfo mfi : dir.entryInfoList()) {
        if (mfi.isDir() && mfi.fileName() != "." && mfi.fileName() != "..") {
            m_comboBox->addItem(itemText(path, mfi.fileName(), mode));
            m_items.append(mfi.fileName());
        }
    }
}

QString SettingsComboBox::itemText(const QString &path, const QString &dirName, const QString &fileName)
{
    QString name("Unknow");

    if (fileName.isEmpty()) {
        name = dirName;
    } else {
        QFile file(path + '/' + dirName + '/' + fileName);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            name = QString(file.readLine()).trimmed();
            file.close();
        }
    }
    return name;
}

SettingsCheckBox::SettingsCheckBox(const QJsonObject &json,
                                   const QString &path,
                                   QWidget *parent)
                                 : AbstractSettingsWidget(json, path, parent)
{
    m_checkBox = new QCheckBox(this);
    m_layout->addWidget(m_checkBox);
    m_text = json.value("text").toString();
    m_checkBox->setText(m_text);
    connect(m_checkBox, &QCheckBox::stateChanged, this, &SettingsCheckBox::onCheckStatusChanged);
}

void SettingsCheckBox::retranslate(const Translate *tr)
{
    m_checkBox->setText(tr->tr(m_text));
}

void SettingsCheckBox::loadSettings(QSettings *config)
{
    m_checkBox->setChecked(config->value(m_key).toBool());
    onCheckStatusChanged();
}

void SettingsCheckBox::saveSettings(QSettings *config)
{
    config->setValue(m_key, m_checkBox->isChecked());
}

void SettingsCheckBox::onCheckStatusChanged()
{
    setMutexStatus(m_checkBox->isChecked());
}

SettingsSlider::SettingsSlider(const QJsonObject &json,
                               const QString &path,
                               QWidget *parent)
                             : AbstractSettingsWidget(json, path, parent)
{
    QSpinBox *spinBox = new QSpinBox(this);

    m_slider = new QSlider(this);
    m_slider->setOrientation(Qt::Horizontal);
    m_layout->addWidget(m_slider);
    m_layout->addWidget(spinBox);

    QStringList list = json.value("range").toString().split(QRegExp(",\\s*"));
    if (list.size() == 2) {
        int lower = list[0].toInt();
        int upper = list[1].toInt();
        m_slider->setRange(lower, upper);
        spinBox->setRange(lower, upper);
    } else {
        m_slider->setRange(0, 100);
    }

    connect(m_slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
}

void SettingsSlider::loadSettings(QSettings *config)
{
    m_slider->setValue(config->value(m_key).toInt());
}

void SettingsSlider::saveSettings(QSettings *config)
{
    config->setValue(m_key, m_slider->value());
}

SettingsFontSelect::SettingsFontSelect(const QJsonObject &json,
                                       const QString &path,
                                       QWidget *parent)
                                     : AbstractSettingsWidget(json, path, parent)
{
    QPushButton *botton = new QPushButton(this);

    m_edit = new QLineEdit(this);
    m_layout->addWidget(m_edit);
    m_layout->addWidget(botton);
    m_edit->setReadOnly(true);
    botton->setText("Font...");

    connect(botton, SIGNAL(released()), this, SLOT(fontDialog()));
}

void SettingsFontSelect::loadSettings(QSettings *config)
{
    m_edit->setText(config->value(m_key).toString());
}

void SettingsFontSelect::saveSettings(QSettings *config)
{
    config->setValue(m_key, m_edit->text());
}

void SettingsFontSelect::fontDialog()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    m_edit->setText(font.family());
}

SettingsFontFamily::SettingsFontFamily(const QJsonObject &json,
                                       const QString &path,
                                       QWidget *parent)
                                     : AbstractSettingsWidget(json, path, parent)
{
    QPushButton *botton = new QPushButton(this);

    m_edit = new QLineEdit(this);
    m_layout->addWidget(m_edit);
    m_layout->addWidget(botton);
    botton->setText("Font...");

    connect(botton, SIGNAL(released()), this, SLOT(fontDialog()));
}

void SettingsFontFamily::loadSettings(QSettings *config)
{
    m_edit->setText(config->value(m_key).toString());
}

void SettingsFontFamily::saveSettings(QSettings *config)
{
    config->setValue(m_key, m_edit->text());
}

void SettingsFontFamily::fontDialog()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    QString fontName = font.family();
    if (fontName.indexOf(' ') >= 0) {
        fontName = "'" + fontName + "'";
    }
    m_edit->setText(fontName);
}

SettingsSpinBox::SettingsSpinBox(const QJsonObject &json,
                                 const QString &path,
                                 QWidget *parent)
                               : AbstractSettingsWidget(json, path, parent)
{
    if (json.value("mode").toString() == "float") {
        QDoubleSpinBox *box = new QDoubleSpinBox(this);
        box->setMinimum(json.value("minimum").toDouble());
        box->setMaximum(json.value("maximum").toDouble());
        m_spinBox = box;
        m_mode = DoubleSpinBox;
    } else {
        QSpinBox *box = new QSpinBox(this);
        box->setMinimum(json.value("minimum").toInt());
        box->setMaximum(json.value("maximum").toInt());
        m_spinBox = box;
        m_mode = IntSpinBox;
    }
    QString sizePolicy(json.value("size-policy").toString());
    if (sizePolicy == "fixed") {
        m_spinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    }
    m_layout->addWidget(m_spinBox);
    m_layout->setAlignment(m_spinBox, Qt::AlignLeft);
}

void SettingsSpinBox::loadSettings(QSettings *config)
{
    if (m_mode == IntSpinBox) {
        dynamic_cast<QSpinBox *>(m_spinBox)->setValue(config->value(m_key).toInt());
    } else {
        dynamic_cast<QDoubleSpinBox *>(m_spinBox)->setValue(config->value(m_key).toDouble());
    }
}

void SettingsSpinBox::saveSettings(QSettings *config)
{
    if (m_mode == IntSpinBox) {
        config->setValue(m_key,  dynamic_cast<QSpinBox *>(m_spinBox)->value());
    } else {
        config->setValue(m_key,  dynamic_cast<QDoubleSpinBox *>(m_spinBox)->value());
    }
}

SettingsLineEdit::SettingsLineEdit(const QJsonObject &json,
                                   const QString &path,
                                   QWidget *parent)
                                 : AbstractSettingsWidget(json, path, parent)
{
    m_lineEdit = new QLineEdit(this);
    m_layout->addWidget(m_lineEdit);
}

void SettingsLineEdit::loadSettings(QSettings *config)
{
    m_lineEdit->setText(config->value(m_key).toString());
}

void SettingsLineEdit::saveSettings(QSettings *config)
{
    config->setValue(m_key,  m_lineEdit->text());
}
