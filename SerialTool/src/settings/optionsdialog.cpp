#include "optionsdialog.h"
#include "settingswidget.h"
#include <QSettings>
#include <QVBoxLayout>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_config = new QSettings("settings.ini", QSettings::IniFormat);
    m_settingsWidget = new SettingsWidget(this);
    m_settingsWidget->layout("../SettingsWidget/resources/layout/index.json");
    m_settingsWidget->loadSettings(m_config);
    layout->addWidget(m_settingsWidget);
    setLayout(layout);
    setWindowTitle(tr("Options"));
    resize(400, 320);
}

void OptionsDialog::closeEvent(QCloseEvent * /* event */)
{
    m_settingsWidget->saveSettings(m_config);
}

OptionsDialog::~OptionsDialog()
{
}
