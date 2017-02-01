/********************************************************************************
** Form generated from reading UI file 'optionsbox.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSBOX_H
#define UI_OPTIONSBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_optionsBox
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QPushButton *fontMultiSetButton;
    QLabel *label_4;
    QPushButton *fontAnsiSetButton;
    QLineEdit *lineEditFontMulti;
    QLineEdit *lineEditFontAnsi;
    QLabel *label;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QPushButton *rxColorButton;
    QLineEdit *lineEditRxColor;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEditTxColor;
    QPushButton *txColorButton;
    QSpacerItem *verticalSpacer;
    QWidget *tab2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QLineEdit *lineEditPlotColor;
    QPushButton *plotBgColorButton;
    QLabel *label_7;
    QLineEdit *lineEditAxisColor;
    QPushButton *axisColorButton;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *plotAntiBox;
    QCheckBox *gridAntiBox;
    QCheckBox *openGlBox;
    QSpacerItem *verticalSpacer_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *optionsBox)
    {
        if (optionsBox->objectName().isEmpty())
            optionsBox->setObjectName(QStringLiteral("optionsBox"));
        optionsBox->resize(400, 312);
        verticalLayout = new QVBoxLayout(optionsBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(optionsBox);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab1 = new QWidget();
        tab1->setObjectName(QStringLiteral("tab1"));
        verticalLayout_2 = new QVBoxLayout(tab1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 9, 3, 3);
        groupBox = new QGroupBox(tab1);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        fontMultiSetButton = new QPushButton(groupBox);
        fontMultiSetButton->setObjectName(QStringLiteral("fontMultiSetButton"));

        gridLayout_2->addWidget(fontMultiSetButton, 1, 2, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        fontAnsiSetButton = new QPushButton(groupBox);
        fontAnsiSetButton->setObjectName(QStringLiteral("fontAnsiSetButton"));

        gridLayout_2->addWidget(fontAnsiSetButton, 0, 2, 1, 1);

        lineEditFontMulti = new QLineEdit(groupBox);
        lineEditFontMulti->setObjectName(QStringLiteral("lineEditFontMulti"));
        lineEditFontMulti->setReadOnly(true);

        gridLayout_2->addWidget(lineEditFontMulti, 1, 1, 1, 1);

        lineEditFontAnsi = new QLineEdit(groupBox);
        lineEditFontAnsi->setObjectName(QStringLiteral("lineEditFontAnsi"));
        lineEditFontAnsi->setReadOnly(true);

        gridLayout_2->addWidget(lineEditFontAnsi, 0, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab1);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        rxColorButton = new QPushButton(groupBox_2);
        rxColorButton->setObjectName(QStringLiteral("rxColorButton"));

        gridLayout->addWidget(rxColorButton, 0, 2, 1, 1);

        lineEditRxColor = new QLineEdit(groupBox_2);
        lineEditRxColor->setObjectName(QStringLiteral("lineEditRxColor"));
        lineEditRxColor->setReadOnly(true);

        gridLayout->addWidget(lineEditRxColor, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        lineEditTxColor = new QLineEdit(groupBox_2);
        lineEditTxColor->setObjectName(QStringLiteral("lineEditTxColor"));
        lineEditTxColor->setReadOnly(true);

        gridLayout->addWidget(lineEditTxColor, 1, 1, 1, 1);

        txColorButton = new QPushButton(groupBox_2);
        txColorButton->setObjectName(QStringLiteral("txColorButton"));

        gridLayout->addWidget(txColorButton, 1, 2, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QStringLiteral("tab2"));
        verticalLayout_3 = new QVBoxLayout(tab2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, -1, 3, 3);
        groupBox_3 = new QGroupBox(tab2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        lineEditPlotColor = new QLineEdit(groupBox_3);
        lineEditPlotColor->setObjectName(QStringLiteral("lineEditPlotColor"));
        lineEditPlotColor->setReadOnly(true);

        gridLayout_3->addWidget(lineEditPlotColor, 0, 1, 1, 1);

        plotBgColorButton = new QPushButton(groupBox_3);
        plotBgColorButton->setObjectName(QStringLiteral("plotBgColorButton"));

        gridLayout_3->addWidget(plotBgColorButton, 0, 2, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_7, 1, 0, 1, 1);

        lineEditAxisColor = new QLineEdit(groupBox_3);
        lineEditAxisColor->setObjectName(QStringLiteral("lineEditAxisColor"));
        lineEditAxisColor->setReadOnly(true);

        gridLayout_3->addWidget(lineEditAxisColor, 1, 1, 1, 1);

        axisColorButton = new QPushButton(groupBox_3);
        axisColorButton->setObjectName(QStringLiteral("axisColorButton"));

        gridLayout_3->addWidget(axisColorButton, 1, 2, 1, 1);


        verticalLayout_3->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(tab2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        plotAntiBox = new QCheckBox(groupBox_4);
        plotAntiBox->setObjectName(QStringLiteral("plotAntiBox"));

        verticalLayout_4->addWidget(plotAntiBox);

        gridAntiBox = new QCheckBox(groupBox_4);
        gridAntiBox->setObjectName(QStringLiteral("gridAntiBox"));

        verticalLayout_4->addWidget(gridAntiBox);

        openGlBox = new QCheckBox(groupBox_4);
        openGlBox->setObjectName(QStringLiteral("openGlBox"));

        verticalLayout_4->addWidget(openGlBox);


        verticalLayout_3->addWidget(groupBox_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        tabWidget->addTab(tab2, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(optionsBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(optionsBox);
        QObject::connect(buttonBox, SIGNAL(accepted()), optionsBox, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), optionsBox, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(optionsBox);
    } // setupUi

    void retranslateUi(QDialog *optionsBox)
    {
        optionsBox->setWindowTitle(QApplication::translate("optionsBox", "\351\200\211\351\241\271", 0));
        groupBox->setTitle(QApplication::translate("optionsBox", "\351\200\211\346\213\251\345\255\227\344\275\223", 0));
        fontMultiSetButton->setText(QApplication::translate("optionsBox", "\351\200\211\346\213\251\345\255\227\344\275\223...", 0));
        label_4->setText(QApplication::translate("optionsBox", "\345\244\232\345\255\227\350\212\202\345\255\227\347\254\246\357\274\232", 0));
        fontAnsiSetButton->setText(QApplication::translate("optionsBox", "\351\200\211\346\213\251\345\255\227\344\275\223...", 0));
        label->setText(QApplication::translate("optionsBox", "ANSI\345\255\227\347\254\246\357\274\232", 0));
        groupBox_2->setTitle(QApplication::translate("optionsBox", "\345\255\227\344\275\223\351\242\234\350\211\262", 0));
        rxColorButton->setText(QApplication::translate("optionsBox", "\351\200\211\346\213\251\351\242\234\350\211\262...", 0));
        label_2->setText(QApplication::translate("optionsBox", "\346\216\245\346\224\266\345\214\272\351\242\234\350\211\262\357\274\232", 0));
        label_3->setText(QApplication::translate("optionsBox", "\345\217\221\351\200\201\345\214\272\351\242\234\350\211\262\357\274\232", 0));
        txColorButton->setText(QApplication::translate("optionsBox", "\351\200\211\346\213\251\351\242\234\350\211\262...", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("optionsBox", "\345\255\227\344\275\223", 0));
        groupBox_3->setTitle(QApplication::translate("optionsBox", "\351\242\234\350\211\262\345\222\214\346\240\267\345\274\217", 0));
        label_5->setText(QApplication::translate("optionsBox", "\350\203\214\346\231\257\351\242\234\350\211\262\357\274\232", 0));
        plotBgColorButton->setText(QApplication::translate("optionsBox", "\351\200\211\346\213\251\351\242\234\350\211\262...", 0));
        label_7->setText(QApplication::translate("optionsBox", "\347\275\221\346\240\274\351\242\234\350\211\262\357\274\232", 0));
        axisColorButton->setText(QApplication::translate("optionsBox", "\351\200\211\346\213\251\351\242\234\350\211\262", 0));
        groupBox_4->setTitle(QApplication::translate("optionsBox", "\345\233\276\345\275\242\351\200\211\351\241\271", 0));
        plotAntiBox->setText(QApplication::translate("optionsBox", "\346\263\242\345\275\242\347\273\230\345\210\266\346\212\227\351\224\257\351\275\277", 0));
        gridAntiBox->setText(QApplication::translate("optionsBox", "\347\275\221\346\240\274\346\212\227\351\224\257\351\275\277", 0));
#ifndef QT_NO_TOOLTIP
        openGlBox->setToolTip(QApplication::translate("optionsBox", "\346\263\250\346\204\217\357\274\214\345\274\200\345\220\257\350\257\245\351\200\211\351\241\271\345\256\236\351\231\205\344\270\212\344\274\232\344\275\277CPU\345\215\240\347\224\250\347\216\207\345\217\230\351\253\230", 0));
#endif // QT_NO_TOOLTIP
        openGlBox->setText(QApplication::translate("optionsBox", "\347\241\254\344\273\266\345\212\240\351\200\237(OpenGL)", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("optionsBox", "\347\244\272\346\263\242\345\231\250", 0));
    } // retranslateUi

};

namespace Ui {
    class optionsBox: public Ui_optionsBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSBOX_H
