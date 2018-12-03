#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QSettings;
class QDialogButtonBox;
class QAbstractButton;
class SettingsWidget;

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QSettings *settings, QWidget *parent = nullptr);
    ~OptionsDialog();

signals:
    void settingsUpdated();

private slots:
    void processOptions(QAbstractButton *button);

private:
    QDialogButtonBox *m_buttonBox;
    SettingsWidget *m_settingsWidget;
    QSettings *m_config;
};

#endif // OPTIONSDIALOG_H
