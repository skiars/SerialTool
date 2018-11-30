#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QTabWidget>

class QSettings;
class AbstractSettingsWidget;
class Translate;
class QGroupBox;
class QLabel;

class SettingsWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();
    void layout(const QString &fileName);
    void loadSettings(QSettings *config);
    void saveSettings(QSettings *config);
    void retranslate(const Translate *translate);

private slots:
    void mutexWidgets(bool mutex, QStringList list);

private:
    bool layoutTabs(const QByteArray &json);
    bool layoutPage(const QByteArray &json);
    QLayout* parseLayout(const QJsonObject &json);
    void layoutPageView(QLayout *layout, const QJsonObject &json);
    void addBlock(QLayout *layout, const QJsonObject &json);
    QWidget* addSettingsWidget(QString type, const QJsonObject &json);

    void parseView(QWidget *widget, const QJsonObject &json);
    QWidget* parseBlock(const QJsonObject &json);
    QWidget* parseGroup(const QJsonObject &json);

private:
    QString m_path;
    QList<QGroupBox *> m_groupBoxs;
    QList<QLabel *> m_labels;
    QList<AbstractSettingsWidget *> m_settingsWidgets;
    Translate *m_translate;
};

#endif // SETTINGSWIDGET_H
