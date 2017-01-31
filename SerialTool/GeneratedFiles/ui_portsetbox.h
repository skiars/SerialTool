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
        dataBitsBox->setObjectName(QStringLiteral("dataBitsBox"));

        gridLayout->addWidget(dataBitsBox, 0, 1, 1, 1);

        label_2 = new QLabel(PortSetBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        paritBox = new QComboBox(PortSetBox);
        paritBox->setObjectName(QStringLiteral("paritBox"));

        gridLayout->addWidget(paritBox, 1, 1, 1, 1);

        label_3 = new QLabel(PortSetBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        stopBitsBox = new QComboBox(PortSetBox);
        stopBitsBox->setObjectName(QStringLiteral("stopBitsBox"));

        gridLayout->addWidget(stopBitsBox, 2, 1, 1, 1);

        label_4 = new QLabel(PortSetBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        flowControlBox = new QComboBox(PortSetBox);
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
        PortSetBox->setWindowTitle(QApplication::translate("PortSetBox", "\347\253\257\345\217\243\345\217\202\346\225\260\350\256\276\347\275\256", 0));
        label->setText(QApplication::translate("PortSetBox", "\346\225\260\346\215\256\344\275\215\357\274\232", 0));
        dataBitsBox->clear();
        dataBitsBox->insertItems(0, QStringList()
         << QApplication::translate("PortSetBox", "5", 0)
         << QApplication::translate("PortSetBox", "6", 0)
         << QApplication::translate("PortSetBox", "7", 0)
         << QApplication::translate("PortSetBox", "8", 0)
        );
        label_2->setText(QApplication::translate("PortSetBox", "\346\240\241\351\252\214\344\275\215\357\274\232", 0));
        paritBox->clear();
        paritBox->insertItems(0, QStringList()
         << QApplication::translate("PortSetBox", "None", 0)
         << QApplication::translate("PortSetBox", "Even", 0)
         << QApplication::translate("PortSetBox", "Odd", 0)
         << QApplication::translate("PortSetBox", "Space", 0)
         << QApplication::translate("PortSetBox", "Mark", 0)
        );
        label_3->setText(QApplication::translate("PortSetBox", "\345\201\234\346\255\242\344\275\215\357\274\232", 0));
        stopBitsBox->clear();
        stopBitsBox->insertItems(0, QStringList()
         << QApplication::translate("PortSetBox", "1", 0)
         << QApplication::translate("PortSetBox", "1.5", 0)
         << QApplication::translate("PortSetBox", "2", 0)
        );
        label_4->setText(QApplication::translate("PortSetBox", "\346\265\201\346\216\247\357\274\232", 0));
        flowControlBox->clear();
        flowControlBox->insertItems(0, QStringList()
         << QApplication::translate("PortSetBox", "None", 0)
         << QApplication::translate("PortSetBox", "RTS/CTS", 0)
         << QApplication::translate("PortSetBox", "XON/XOFF", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class PortSetBox: public Ui_PortSetBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTSETBOX_H
