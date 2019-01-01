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

    m_translate = new Translate();
    m_path = fileName.left(fileName.lastIndexOf('/'));
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        layoutTabs(file.readAll());
        file.close();
    } else {
        qDebug() << "This file" << fileName << "is not found.";
    }
}

void SettingsWidget::loadSettings(QSettings *config)
{
    config->beginGroup("Settings");
    retranslate(config->value("Language").toString());
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
    retranslate(config->value("Language").toString());
    config->endGroup();
}

void SettingsWidget::retranslate(const QString &language)
{
    if (!language.isEmpty()) {
        m_translate->setTranslateFile("./language/" + language + "/settings.txt");
    }
    for (int i = 0; i < count(); ++i) {
        setTabText(i, m_translate->tr(m_tabsName[i]));
    }
    for (int i = 0; i < m_labels.size(); ++i) {
        m_labels[i]->setText(m_translate->tr(m_labelsName[i]));
    }
    for (int i = 0; i < m_groupBoxs.size(); ++i) {
        m_groupBoxs[i]->setTitle(m_translate->tr(m_groupBoxsName[i]));
    }
    for (AbstractSettingsWidget *widget : m_settingsWidgets) {
        widget->retranslate(m_translate);
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
        m_currentTab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(m_currentTab);
        layout->setMargin(6);
        layoutPageView(layout, object);
        layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
        m_currentTab->setLayout(layout);
        this->addTab(m_currentTab, title);
        m_tabsName.append(title);
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
        layout = new QVBoxLayout(m_currentTab);
    } else if (type == "hbox") {
        layout = new QHBoxLayout(m_currentTab);
    } else if (type == "grid") {
        layout = new QGridLayout(m_currentTab);
    } else if (type == "form") {
        QFormLayout *formLayout = new QFormLayout(m_currentTab);
        formLayout->setLabelAlignment(Qt::AlignRight);
        layout = formLayout;
    } else {
        layout = new QVBoxLayout(m_currentTab);
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
        widget = new SettingsComboBox(json, m_path, m_currentTab);
    } else if (type == "check-box") {
        widget = new SettingsCheckBox(json, m_path, m_currentTab);
    } else if (type == "slider") {
        widget = new SettingsSlider(json, m_path, m_currentTab);
    } else if (type == "font-select") {
        widget = new SettingsFontSelect(json, m_path, m_currentTab);
    } else if (type == "font-family") {
        widget = new SettingsFontFamily(json, m_path, m_currentTab);
    } else if (type == "spin-box") {
        widget = new SettingsSpinBox(json, m_path, m_currentTab);
    } else if (type == "line-edit") {
        widget = new SettingsLineEdit(json, m_path, m_currentTab);
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
                QLabel *label = new QLabel(text, m_currentTab);
                formLayout->addRow(label, widget);
                m_labels.append(label);
                m_labelsName.append(text);
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
    QWidget *widget = new QWidget(m_currentTab);

    parseView(widget, json);
    return widget;
}

QWidget* SettingsWidget::parseGroup(const QJsonObject &json)
{
    QGroupBox *widget = new QGroupBox(m_currentTab);
    QString title(json.value("title").toString());
    widget->setTitle(title);
    parseView(widget, json);
    m_groupBoxs.append(widget);
    m_groupBoxsName.append(title);
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
