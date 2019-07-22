#include "scriptextensionview.h"
#include <QtScript/QScriptEngine>
#include <QUiLoader>
#include <QHBoxLayout>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QPushButton>

ScriptExtensionView::ScriptExtensionView(const QString &fileName, QWidget *parent) :
    AbstractView(parent),
    m_script(new QScriptEngine)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        loadUi(fileName.left(fileName.size() - 3) + ".ui");
        QScriptValue scriptObj = m_script->newQObject(this);
        m_script->globalObject().setProperty("view", scriptObj);
        m_script->evaluate(file.readAll());
        file.close();
    }
    m_script->evaluate("view.init()");
}

void ScriptExtensionView::loadUi(const QString &fileName)
{
    QFile uiFile(fileName);
    if (uiFile.open(QIODevice::ReadOnly)) {
        QUiLoader loader;
        QHBoxLayout *layout = new QHBoxLayout(this);
        QWidget *widget = loader.load(&uiFile, this);
        layout->addWidget(widget);
        setLayout(layout);
    }
}

void ScriptExtensionView::transmitData(const QString &data)
{
    emit AbstractView::transmitData(data.toLocal8Bit());
}

void ScriptExtensionView::sendMessage(const QString &receiver, const QString &message)
{
    emit AbstractView::sendMessage(receiver, message.toLocal8Bit());
}

QString ScriptExtensionView::title()
{
    return m_script->evaluate("view.title()").toString();
}

QString ScriptExtensionView::iid()
{
    return m_script->evaluate("view.iid()").toString();
}

void ScriptExtensionView::clear()
{
    m_script->evaluate("view.clear()");
}

void ScriptExtensionView::loadConfig(QSettings *config)
{
    QScriptValue view = m_script->evaluate("view");
    view.property("loadConfig").call(view,
        QScriptValueList() << m_script->newQObject(config));
}

void ScriptExtensionView::saveConfig(QSettings *config)
{
    QScriptValue view = m_script->evaluate("view");
    view.property("saveConfig").call(view,
        QScriptValueList() << m_script->newQObject(config));
}

void ScriptExtensionView::loadSettings(QSettings *config)
{
    QScriptValue view = m_script->evaluate("view");
    view.property("loadSettings").call(view,
        QScriptValueList() << m_script->newQObject(config));
}

void ScriptExtensionView::retranslate()
{
    m_script->evaluate("view.retranslate()");
}


void ScriptExtensionView::receiveData(const QByteArray &array)
{
    QScriptValue view = m_script->evaluate("view");
    QString string(QString::fromLocal8Bit(array));
    view.property("receiveData").call(view, QScriptValueList() << string);
}

void ScriptExtensionView::setEnabled(bool enabled)
{
    QScriptValue view = m_script->evaluate("view");
    view.property("setEnabled").call(view, QScriptValueList() << enabled);
}

QString ScriptExtensionView::openFileFilter()
{
    return m_script->evaluate("view.openFileFilter()").toString();
}

QString ScriptExtensionView::saveFileFilter()
{
    return m_script->evaluate("view.openFileFilter()").toString();
}

void ScriptExtensionView::saveFile(const QString &fileName, const QString &filter)
{
    QScriptValue view = m_script->evaluate("view");
    view.property("saveFile").call(view, QScriptValueList() << fileName << filter);
}

void ScriptExtensionView::openFile(const QString &fileName, const QString &filter)
{
    QScriptValue view = m_script->evaluate("view");
    view.property("openFile").call(view, QScriptValueList() << fileName << filter);
}

void ScriptExtensionView::takeMessage(const QString &sender, const QByteArray &message)
{
    QScriptValue view = m_script->evaluate("view");
    view.property("takeMessage").call(view,
        QScriptValueList() << sender << m_script->newVariant(message));
}
