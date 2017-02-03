/********************************************************************************
** Form generated from reading UI file 'portsetbox.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PORTSETBOX_H
#define UI_PORTSETBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_PortSetBox
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *dataBitsBox;
    QLabel *label_2;
    QComboBox *paritBox;
    QLabel *label_3;
    QComboBox *stopBitsBox;
    QLabel *label_4;
    QComboBox *flowControlBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PortSetBox)
    {
        if (PortSetBox->objectName().isEmpty())
            PortSetBox->setObjectName(QStringLiteral("PortSetBox"));
        PortSetBox->resize(200, 145);
        gridLayout = new QGridLayout(PortSetBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(PortSetBox);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        dataBitsBox = new QComboBox(PortSetBox);
        dataBitsBox->insertItems(0, QStringList()
         << QStringLiteral("5")
         << QStringLiteral("6")
         << QStringLiteral("7")
         << QStringLiteral("8")
        );
        dataBitsBox->setObjectName(QStringLiteral("dataBitsBox"));

        gridLayout->addWidget(dataBitsBox, 0, 1, 1, 1);

        label_2 = new QLabel(PortSetBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        paritBox = new QComboBox(PortSetBox);
        paritBox->insertItems(0, QStringList()
         << QStringLiteral("None")
         << QStringLiteral("Even")
         << QStringLiteral("Odd")
         << QStringLiteral("Space")
         << QStringLiteral("Mark")
        );
        paritBox->setObjectName(QStringLiteral("paritBox"));

        gridLayout->addWidget(paritBox, 1, 1, 1, 1);

        label_3 = new QLabel(PortSetBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        stopBitsBox = new QComboBox(PortSetBox);
        stopBitsBox->insertItems(0, QStringList()
         << QStringLiteral("1")
         << QStringLiteral("1.5")
         << QStringLiteral("2")
        );
        stopBitsBox->setObjectName(QStringLiteral("stopBitsBox"));

        gridLayout->addWidget(stopBitsBox, 2, 1, 1, 1);

        label_4 = new QLabel(PortSetBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        flowControlBox = new QComboBox(PortSetBox);
        flowControlBox->insertItems(0, QStringList()
         << QStringLiteral("None")
         << QStringLiteral("RTS/CTS")
         << QStringLiteral("XON/XOFF")
        );
        flowControlBox->setObjectName(QStringLiteral("flowControlBox"));

        gridLayout->addWidget(flowControlBox, 3, 1, 1, 1);

        buttonBox = new QDialogButtonBox(PortSetBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 4, 1, 1, 1);


        retranslateUi(PortSetBox);
        QObject::connect(buttonBox, SIGNAL(accepted()), PortSetBox, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PortSetBox, SLOT(reject()));

        QMetaObject::connectSlotsByName(PortSetBox);
    } // setupUi

    void retranslateUi(QDialog *PortSetBox)
    {
        PortSetBox->setWindowTitle(QApplication::translate("PortSetBox", "Port Settings", 0));
        label->setText(QApplication::translate("PortSetBox", "Data Bits", 0));
        label_2->setText(QApplication::translate("PortSetBox", "Parity", 0));
        label_3->setText(QApplication::translate("PortSetBox", "Stop Bits", 0));
        label_4->setText(QApplication::translate("PortSetBox", "Flow Control", 0));
    } // retranslateUi

};

namespace Ui {
    class PortSetBox: public Ui_PortSetBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTSETBOX_H
