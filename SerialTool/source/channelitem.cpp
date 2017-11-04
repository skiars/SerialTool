#include "channelitem.h"
#include <QColorDialog>

ChannelItem::ChannelItem(QString string, QColor color, QWidget *parent) : QWidget(parent)
{
    colorBtn = new QToolButton;
    colorBtn->setFixedSize(QSize(16, 16));
    colorBtn->setAutoRaise(true);
    QAction *action = new QAction(this);
    action->setToolTip(tr("Change plot lines color"));
    colorBtn->setDefaultAction(action);
    setColor(color);

    checkBox = new QCheckBox(string, this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(checkBox);
    layout->addWidget(colorBtn);
    setLayout(layout);
    connect(colorBtn, &QToolButton::clicked, this, &ChannelItem::changeColor);
    connect(checkBox, &QCheckBox::stateChanged, this, &ChannelItem::itemChange);
}

ChannelItem::~ChannelItem()
{
    delete colorBtn;
    delete checkBox;
}

void ChannelItem::itemChange()
{
    emit changelChanged(this);
}

void ChannelItem::setColor(QColor color)
{
    Color = color;
    QPixmap pixmap(16, 16);
    QPainter painter(&pixmap);
    painter.setPen(QColor(50, 50, 50));
    painter.drawRect(QRect(0, 0, 15, 15));
    painter.fillRect(QRect(1, 1, 14, 14), color);

    colorBtn->defaultAction()->setIcon(QIcon(pixmap));
}

void ChannelItem::changeColor()
{
    QColor color = QColorDialog::getColor(QColor(Color), this);

    if (color.isValid()) {
        setColor(color);
        emit changelChanged(this);
    }
}
