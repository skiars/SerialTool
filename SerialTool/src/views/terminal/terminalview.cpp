#include "terminalview.h"
#include "termview.h"
#include <QHBoxLayout>
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

void TerminalView::sendData(const QString &string)
{
    emit transmitData(string.toLocal8Bit());
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
