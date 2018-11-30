#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QSettings;

class SettingsWidget;

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();

private:
    void closeEvent(QCloseEvent *event);

private:
    SettingsWidget *m_settingsWidget;
    QSettings *m_config;
};

#endif // OPTIONSDIALOG_H
