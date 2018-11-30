#include "settingswidget.h"
#include "settingswidgets.h"
#include "translate.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QSettings>
#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QFile>
#include <QDebug>

SettingsWidget::SettingsWidget(QWidget *parent) : QTabWidget(parent)
{
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::layout(const QString &fileName)
{
    QFile file(fileName);

    m_path = fileName.left(fileName.lastIndexOf('/'));
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        layoutTabs(file.readAll());
        file.close();
    } else {
        qDebug() << "This file" << fileName << "is not found.";
    }

    m_translate = new Translate("../SettingsWidget/resources/translate.txt");
    retranslate(m_translate);
}

void SettingsWidget::loadSettings(QSettings *config)
{
    config->beginGroup("Settings");
    for (AbstractSettingsWidget *widget : m_settingsWidgets) {
        widget->loadSettings(config);
    }
    config->endGroup();
}

void SettingsWidget::saveSettings(QSettings *config)
{
    config->beginGroup("Settings");
    for (AbstractSettingsWidget *widget : m_settingsWidgets) {
        widget->saveSettings(config);
    }
    config->endGroup();
}

void SettingsWidget::retranslate(const Translate *translate)
{
    for (int i = 0; i < count(); ++i) {
        setTabText(i, translate->tr(tabText(i)));
    }
    for (QLabel *label : m_labels) {
        label->setText(translate->tr(label->text()));
    }
    for (QGroupBox *box : m_groupBoxs) {
        box->setTitle(translate->tr(box->title()));
    }
    for (AbstractSettingsWidget *widget : m_settingsWidgets) {
        widget->retranslate(translate);
    }
}

bool SettingsWidget::layoutTabs(const QByteArray &json)
{
    QJsonParseError jsonError;
    QJsonDocument docment = QJsonDocument::fromJson(json, &jsonError);

    if (jsonError.error == QJsonParseError::NoError && docment.isObject()) {
        QJsonObject object = docment.object();
        QJsonValue value = object.value("tabs");

        if (value.isArray()) {
            QJsonArray array = value.toArray();

            for (QJsonValue value : array) {
                QString fileName = m_path + '/' + value.toString();
                QFile file(fileName);

                if (file.open(QFile::ReadOnly | QFile::Text)) {
                    layoutPage(file.readAll());
                    file.close();
                } else {
                    qDebug() << "This file" << fileName << "is not found.";
                }
            }
        } else {
            qDebug() << "'tabs' must be array.";
        }
    } else {
        qDebug() << "JSON Parser error:" << jsonError.error;
        return false;
    }
    return true;
}

bool SettingsWidget::layoutPage(const QByteArray &json)
{
    QJsonParseError jsonError;
    QJsonDocument docment = QJsonDocument::fromJson(json, &jsonError);

    if (jsonError.error == QJsonParseError::NoError && docment.isObject()) {
        QJsonObject object = docment.object();
        QJsonValue value = object.value("title");
        QString title = value.isString() ? value.toString() : "Unknow";
        QWidget *widget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(this);

        layout->setMargin(6);
        widget->setLayout(layout);
        this->addTab(widget, title);
        layoutPageView(layout, object);
        layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    } else {
        qDebug() << "JSON Parser error:" << jsonError.error;
        return false;
    }
    return true;
}

QLayout* SettingsWidget::parseLayout(const QJsonObject &json)
{
    QLayout *layout;
    QString type = json.value("layout").toString();

    if (type == "vbox") {
        layout = new QVBoxLayout(this);
    } else if (type == "hbox") {
        layout = new QHBoxLayout(this);
    } else if (type == "grid") {
        layout = new QGridLayout(this);
    } else if (type == "form") {
        QFormLayout *formLayout = new QFormLayout(this);
        formLayout->setLabelAlignment(Qt::AlignRight);
        layout = formLayout;
    } else {
        layout = new QVBoxLayout(this);
    }
    layout->setMargin(6);
    layout->setSpacing(6);
    return layout;
}

void SettingsWidget::layoutPageView(QLayout *layout, const QJsonObject &json)
{
    QJsonValue value = json.value("view");

    if (value.isArray()) {
        QJsonArray array = value.toArray();
        for (QJsonValue v : array) {
            if (v.isObject()) {
                addBlock(layout, v.toObject());
            }
        }
    } else {
        qDebug() << "JSON invalid array.";
    }
}

QWidget* SettingsWidget::addSettingsWidget(QString type, const QJsonObject &json)
{
    AbstractSettingsWidget *widget = nullptr;

    if (type == "combo-box") {
        widget = new SettingsComboBox(json, m_path, this);
    } else if (type == "check-box") {
        widget = new SettingsCheckBox(json, m_path, this);
    } else if (type == "slider") {
        widget = new SettingsSlider(json, m_path, this);
    } else if (type == "font-select") {
        widget = new SettingsFontSelect(json, m_path, this);
    } else if (type == "font-family") {
        widget = new SettingsFontFamily(json, m_path, this);
    } else if (type == "spin-box") {
        widget = new SettingsSpinBox(json, m_path, this);
    }

    if (widget) {
        m_settingsWidgets.append(widget);
        connect(widget, &AbstractSettingsWidget::mutexChanged, this, &SettingsWidget::mutexWidgets);
    }
    return widget;
}

void SettingsWidget::addBlock(QLayout *layout, const QJsonObject &json)
{
    QWidget *widget = nullptr;
    QJsonValue value = json.value("type");
    QString type = value.toString();

    if (type == "block") {
         widget = parseBlock(json);
    } else if (type == "group") {
        widget = parseGroup(json);
    } else {
        widget = addSettingsWidget(type, json);
    }

    if (widget == nullptr) {
        qDebug() << "Invalid type:" << type;
        return;
    }

    // layout
    if (widget) {
        if (QFormLayout *formLayout = dynamic_cast<QFormLayout *>(layout)) {
            QString text = json.value("label").toString();
            if (text.isEmpty()) {
                layout->addWidget(widget);
            } else {
                QLabel *label = new QLabel(text, this);
                formLayout->addRow(label, widget);
                m_labels.append(label);
            }
        } else if (QGridLayout *gridLayout = dynamic_cast<QGridLayout *>(layout)) {
            QStringList list = json.value("at").toString().split(QRegExp(",\\s*"));
            if (list.size() == 2) {
                gridLayout->addWidget(widget, list[0].toInt(), list[1].toInt());
            } else {
                layout->addWidget(widget);
            }
        } else {
            layout->addWidget(widget);
        }
    }
}

void SettingsWidget::parseView(QWidget *widget, const QJsonObject &json)
{
    QJsonValue value = json.value("view");

    if (value.isObject()) {
        QLayout *viewLayout = parseLayout(json);
        widget->setLayout(viewLayout);
        addBlock(viewLayout, value.toObject());
    } else if (value.isArray()) {
        QJsonArray array = value.toArray();
        QLayout *viewLayout = parseLayout(json);
        widget->setLayout(viewLayout);
        for (QJsonValue v : array) {
            if (v.isObject()) {
                addBlock(viewLayout, v.toObject());
            }
        }
    }
}

QWidget* SettingsWidget::parseBlock(const QJsonObject &json)
{
    QWidget *widget = new QWidget(this);

    parseView(widget, json);
    return widget;
}

QWidget* SettingsWidget::parseGroup(const QJsonObject &json)
{
    QGroupBox *widget = new QGroupBox(this);

    widget->setTitle(json.value("title").toString());
    parseView(widget, json);
    m_groupBoxs.append(widget);
    return widget;
}

void SettingsWidget::mutexWidgets(bool mutex, QStringList list)
{
    AbstractSettingsWidget *send = dynamic_cast<AbstractSettingsWidget *>(sender());

    for (AbstractSettingsWidget *widget : m_settingsWidgets) {
        if (send != widget && std::binary_search(list.begin(), list.end(), widget->id())) {
            widget->setEnabled(mutex, send);
        }
    }
}
