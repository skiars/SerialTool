#ifndef __CHANNELITEM_H
#define __CHANNELITEM_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QToolButton>
#include <QtGui/QPainter>

class ChannelItem : public QWidget {
    Q_OBJECT

public:
    ChannelItem(QString string,
        QColor color = QColor(255, 255, 255), QWidget *parent = Q_NULLPTR);
    ~ChannelItem();

    Qt::CheckState checkState() {
        return checkBox->checkState();
    }

    bool isChecked() {
        return checkBox->isChecked();
    }

    QColor color() {
        return Color;
    }

    int channel() {
        return Channel;
    }

    void setChecked(bool stats) {
        checkBox->setChecked(stats);
    }

    void setColor(QColor color);

    void setChannel(int ch) {
        Channel = ch;
    }

    void edit();

private slots:
    void itemChange();
    void changeColor();

signals:
    void changelChanged(ChannelItem *item);

private:
    QToolButton *colorBtn;
    QCheckBox   *checkBox;
    QColor Color;
    int Channel;
};

#endif
