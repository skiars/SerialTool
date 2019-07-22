#ifndef TERMINALVIEW_H
#define TERMINALVIEW_H

#include "../abstractview.h"

class QVTerminal;

class TerminalView : public AbstractView
{
    Q_OBJECT

public:
    TerminalView(QWidget *parent = nullptr);
    ~TerminalView();

    QString title() { return tr("Terminal"); }
    QString iid() { return "terminal"; }
    void loadSettings(QSettings *config);
    void receiveData(const QByteArray &array);
    void setEnabled(bool enabled);
    void clear();

private slots:
    void sendData(const QString &string);

private:
    QVTerminal *m_term;
};

#endif // TERMINALVIEW_H
