#include "terminalview.h"
#include "termview.h"
#include <QHBoxLayout>
#include <QTextCodec>
#include <QSettings>
#include <QDebug>

TerminalView::TerminalView(QWidget *parent) : AbstractView(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    m_termView = new TermView(this);
    layout->addWidget(m_termView);
    this->setLayout(layout);

    m_termView->setStyleSheet("font-family: 'Courier New'");

    connect(m_termView, SIGNAL(enterNewline(const QString &)), this, SLOT(sendData(const QString &)));
}

TerminalView::~TerminalView()
{

}

void TerminalView::loadSettings(QSettings *config)
{
    QString fontFamily("'" + config->value("FontFamily").toString().replace("+", "','") + "'");
    int fontSize = config->value("FontSize").toInt();

    fontSize = fontSize < 6 ? 10 : fontSize;
    m_termView->setStyleSheet("font-family: " + fontFamily + ";" +
                              "font-size: " + QString::number(fontSize) + "pt;");
}

void TerminalView::sendData(const QString &string)
{
    QTextCodec *code = QTextCodec::codecForName("GB-2312");
    QByteArray array = code->fromUnicode(string);
    emit transmitData(array);
}

void TerminalView::receiveData(const QByteArray &array)
{
    m_termView->append(QString::fromLocal8Bit(array));
}

void TerminalView::setEnabled(bool enabled)
{
    m_termView->setEnabled(enabled);
}

void TerminalView::clear()
{
    m_termView->clear();
}
