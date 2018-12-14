#ifndef SCRIPTEXTENSIONVIEW_H
#define SCRIPTEXTENSIONVIEW_H

#include "../abstractview.h"

class QScriptEngine;

class ScriptExtensionView : public AbstractView
{
    Q_OBJECT
    Q_PROPERTY (bool enabled WRITE setEnabled READ isEnabled )
public:
    explicit ScriptExtensionView(const QString &fileName, QWidget *parent = nullptr);
    QString title();
    QString iid();
    void clear();
    void loadConfig(QSettings *config);
    void saveConfig(QSettings *config);
    void loadSettings(QSettings *config);
    void retranslate();
    void receiveData(const QByteArray &array);
    void setEnabled(bool enabled);

    QString openFileFilter();
    QString saveFileFilter();
    void saveFile(const QString &fileName, const QString &filter);
    void openFile(const QString &fileName, const QString &filter);

    void takeMessage(const QString &sender, const QByteArray &message);

public:
    Q_INVOKABLE void transmitData(const QString &data);
    Q_INVOKABLE void sendMessage(const QString &receiver, const QString &message);

private:
    void loadUi(const QString &fileName);

private:
    QScriptEngine *m_script;
};

#endif // SCRIPTEXTENSIONVIEW_H
