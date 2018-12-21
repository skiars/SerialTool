#include "optionsdialog.h"
#include "settingswidget.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDebug>

OptionsDialog::OptionsDialog(QSettings *settings, QWidget *parent) :
    QDialog(parent)
{
    m_config = settings;
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    m_settingsWidget = new SettingsWidget(this);
    m_settingsWidget->layout("./config/layout/settings/index.json");
    m_settingsWidget->loadSettings(m_config);
    layout->addWidget(m_settingsWidget);

    m_buttonBox = new QDialogButtonBox(this);
    m_buttonBox->setObjectName("m_buttonBox");
    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Apply | QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    layout->addWidget(m_buttonBox);

    setWindowTitle(tr("Options"));
    resize(400, 320);

    connect(m_buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(processOptions(QAbstractButton *)));
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

OptionsDialog::~OptionsDialog()
{
}


// 保存配置
void OptionsDialog::processOptions(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton btn = m_buttonBox->standardButton(button);

    if (btn == QDialogButtonBox::Ok || btn == QDialogButtonBox::Apply) {
        m_settingsWidget->saveSettings(m_config); // 保存配置
        emit settingsUpdated(); // 配置生效
    }
}

