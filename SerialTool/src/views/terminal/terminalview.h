#ifndef TERMINALVIEW_H
#define TERMINALVIEW_H

#include "../abstractview.h"

class TermView;

class TerminalView : public AbstractView
{
    Q_OBJECT

public:
    TerminalView(QWidget *parent = nullptr);
    ~TerminalView();

    QString title() { return tr("Terminal"); }
    void loadSettings(QSettings *config);
    void receiveData(const QByteArray &array);
    void setEnabled(bool enabled);
    void clear();

private slots:
    void sendData(const QString &string);

private:
    TermView *m_termView;
};

#endif // TERMINALVIEW_H
