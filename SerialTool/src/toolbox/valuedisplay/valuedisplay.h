#ifndef VALUEDISPLAY_H
#define VALUEDISPLAY_H

#include "../abstracttoolbox.h"

namespace Ui {
class ValueDisplay;
}

class ValueDisplay : public AbstractToolBox
{
    Q_OBJECT

public:
    explicit ValueDisplay(QWidget *parent = nullptr);
    ~ValueDisplay();
    void retranslate();
    void receiveData(const QByteArray &array);

private:
    Ui::ValueDisplay *ui;
    QByteArray m_array;
};

class ValueDisplayFactory : public AbstractToolBoxFactory
{
    Q_OBJECT
public:
    AbstractToolBox* create(QWidget *parent = nullptr) { return new ValueDisplay(parent); }
    QString title() { return tr("Value Display"); }
};

#endif // VALUEDISPLAY_H
