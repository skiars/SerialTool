#ifndef ABSTRACTSETTINGSWIDGET_H
#define ABSTRACTSETTINGSWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVariant>

class QSettings;
class QComboBox;
class QCheckBox;
class QSlider;
class QLineEdit;
class QAbstractSpinBox;
class Translate;

class AbstractSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractSettingsWidget(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    virtual void loadSettings(QSettings *config) = 0;
    virtual void saveSettings(QSettings *config) = 0;
    virtual void retranslate(const Translate * /* tr */) {}
    QString id() { return m_id; }
    void setEnabled(bool enabled, const void *widget);

signals:
    void mutexChanged(bool mutex, QStringList list);

protected slots:
    void setMutexStatus(const QVariant &status);

private:
    virtual QHBoxLayout *createLayout() { return new QHBoxLayout(this); }

protected:
    QVariant m_mutexActiveStatus;
    QHBoxLayout *m_layout;
    QString m_key, m_path, m_id;
    QStringList m_mutexList;
    QList<uint32_t> m_disableList;
};

class SettingsComboBox : public AbstractSettingsWidget
{
public:
    explicit SettingsComboBox(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);

private:
    void parseItems(const QString &str);
    QString itemText(const QString &path, const QString &dirName, const QString &fileName);

private:
    QComboBox *m_comboBox;
    QVector<QString> m_items;
};

class SettingsCheckBox : public AbstractSettingsWidget
{
    Q_OBJECT

public:
    explicit SettingsCheckBox(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);
    void retranslate(const Translate *tr);

private slots:
    void onCheckStatusChanged();

private:
    QString m_text;
    QCheckBox *m_checkBox;
};

class SettingsSlider : public AbstractSettingsWidget
{
public:
    explicit SettingsSlider(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);

private:
    QSlider *m_slider;
};

class SettingsFontSelect : public AbstractSettingsWidget
{
    Q_OBJECT

public:
    explicit SettingsFontSelect(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);

private slots:
    void fontDialog();

private:
    QLineEdit *m_edit;
};

class SettingsFontFamily : public AbstractSettingsWidget
{
    Q_OBJECT

public:
    explicit SettingsFontFamily(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);

private slots:
    void fontDialog();

private:
    QLineEdit *m_edit;
};

class SettingsSpinBox : public AbstractSettingsWidget
{
public:
    explicit SettingsSpinBox(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);

private:
    enum {
        IntSpinBox,
        DoubleSpinBox
    } m_mode;
    QAbstractSpinBox *m_spinBox;
};

class SettingsLineEdit : public AbstractSettingsWidget
{
public:
    explicit SettingsLineEdit(const QJsonObject &json,
            const QString &path, QWidget *parent = nullptr);

    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);

private:
    QLineEdit *m_lineEdit;
};

#endif // ABSTRACTSETTINGSWIDGET_H
