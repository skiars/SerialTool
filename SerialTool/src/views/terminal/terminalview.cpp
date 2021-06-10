#include "terminalview.h"
#include "qvterminal/qvterminal.h"
#include <QHBoxLayout>
#include <QTextCodec>
#include <QSettings>
#include <QDebug>

TerminalView::TerminalView(QWidget *parent) : AbstractView(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    
    m_term = new QVTerminal(this);
    m_term->setCursor(Qt :: IBeamCursor);
    m_term->format()->font()->setFamily("Consolas");

    layout->addWidget(m_term);
    layout->setMargin(2);
    
    this->setLayout(layout);

    connect(m_term, SIGNAL(transmitData(const QByteArray &)), this, SIGNAL(transmitData(const QByteArray &)));
}

TerminalView::~TerminalView()
{

}

void TerminalView::loadSettings(QSettings *config)
{
    QString fontFamily("'" + config->value("FontFamily").toString().replace("+", "','") + "'");
    int fontSize = config->value("FontSize").toInt();

    fontSize = fontSize < 6 ? 10 : fontSize;
    //m_termView->setStyleSheet(BASE_STYLE
    //                          "font-family: " + fontFamily + ";" +
    //                          "font-size: " + QString::number(fontSize) + "pt;");

    // text codec
    m_term->setTextCodec(config->value("TerminalTextCodec").toString());
    m_term->setTabWidth(config->value("TerminalTabWidth").toInt());
}

void TerminalView::sendData(const QString &string)
{
    QTextCodec *code = QTextCodec::codecForName("GB-2312");
    QByteArray array = code->fromUnicode(string);
    emit transmitData(array);
}

void TerminalView::receiveData(const QByteArray &array)
{
    m_term->appendData(array);
}

void TerminalView::setEnabled(bool enabled)
{
    m_term->setEnabled(enabled);
}

void TerminalView::clear()
{
    m_term->clear();
}
