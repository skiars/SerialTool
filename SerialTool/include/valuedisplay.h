#ifndef VALUEDISPLAY_H
#define VALUEDISPLAY_H

#include <QDialog>

namespace Ui {
class ValueDisplay;
}

class ValueDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit ValueDisplay(QWidget *parent = 0);
    ~ValueDisplay();
    void append(const QByteArray &array);

private:
    Ui::ValueDisplay *ui;
    QByteArray m_array;
};

#endif // VALUEDISPLAY_H
