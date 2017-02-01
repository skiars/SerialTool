/********************************************************************************
** Form generated from reading UI file 'serialtool.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALTOOL_H
#define UI_SERIALTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_SerialToolClass
{
public:
    QAction *actionSave;
    QAction *actionOption;
    QAction *openPortAction;
    QAction *portRunAction;
    QAction *clearAction;
    QAction *portSetAction;
    QAction *actionClose;
    QAction *actionVisibleToolbar;
    QAction *actionVisibleTab0;
    QAction *actionVisibleTab1;
    QAction *actionVisibleStatusBar;
    QAction *actionAbout;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *portConfigWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBoxPortNum;
    QLabel *label_2;
    QComboBox *comboBoxBaudRate;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QTextEdit *textEditRead;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox_2;
    QRadioButton *portReadHex;
    QRadioButton *portReadAscii;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QRadioButton *portWriteHex;
    QCheckBox *resendBox;
    QLabel *label_3;
    QSpinBox *spinBoxStepTime;
    QRadioButton *portWriteAscii;
    QGridLayout *gridLayout_3;
    QPushButton *sendButton;
    QTextEdit *textEditWrite;
    QComboBox *comboBox;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QCustomPlot *customPlot;
    QScrollBar *horizontalScrollBar;
    QVBoxLayout *verticalLayout_9;
    QListWidget *channelList;
    QLabel *label_4;
    QDoubleSpinBox *yRateUpperBox;
    QLabel *label_5;
    QDoubleSpinBox *yRateLowerBox;
    QLabel *label_6;
    QComboBox *xRangeBox;
    QCheckBox *holdRxOscBox;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QToolBar *toolBar1;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SerialToolClass)
    {
        if (SerialToolClass->objectName().isEmpty())
            SerialToolClass->setObjectName(QStringLiteral("SerialToolClass"));
        SerialToolClass->resize(600, 426);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SerialToolClass->sizePolicy().hasHeightForWidth());
        SerialToolClass->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/SerialTool/images/icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SerialToolClass->setWindowIcon(icon);
        actionSave = new QAction(SerialToolClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SerialTool/images/save.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionOption = new QAction(SerialToolClass);
        actionOption->setObjectName(QStringLiteral("actionOption"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/SerialTool/images/config.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionOption->setIcon(icon2);
        openPortAction = new QAction(SerialToolClass);
        openPortAction->setObjectName(QStringLiteral("openPortAction"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/SerialTool/images/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        openPortAction->setIcon(icon3);
        portRunAction = new QAction(SerialToolClass);
        portRunAction->setObjectName(QStringLiteral("portRunAction"));
        portRunAction->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/SerialTool/images/pause.ico"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QStringLiteral(":/SerialTool/images/pause.png"), QSize(), QIcon::Active, QIcon::Off);
        portRunAction->setIcon(icon4);
        clearAction = new QAction(SerialToolClass);
        clearAction->setObjectName(QStringLiteral("clearAction"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/SerialTool/images/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearAction->setIcon(icon5);
        portSetAction = new QAction(SerialToolClass);
        portSetAction->setObjectName(QStringLiteral("portSetAction"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/SerialTool/images/port config.png"), QSize(), QIcon::Normal, QIcon::Off);
        portSetAction->setIcon(icon6);
        actionClose = new QAction(SerialToolClass);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/SerialTool/images/exit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon7);
        actionVisibleToolbar = new QAction(SerialToolClass);
        actionVisibleToolbar->setObjectName(QStringLiteral("actionVisibleToolbar"));
        actionVisibleToolbar->setCheckable(true);
        actionVisibleToolbar->setChecked(true);
        actionVisibleTab0 = new QAction(SerialToolClass);
        actionVisibleTab0->setObjectName(QStringLiteral("actionVisibleTab0"));
        actionVisibleTab0->setCheckable(true);
        actionVisibleTab1 = new QAction(SerialToolClass);
        actionVisibleTab1->setObjectName(QStringLiteral("actionVisibleTab1"));
        actionVisibleTab1->setCheckable(true);
        actionVisibleStatusBar = new QAction(SerialToolClass);
        actionVisibleStatusBar->setObjectName(QStringLiteral("actionVisibleStatusBar"));
        actionVisibleStatusBar->setCheckable(true);
        actionVisibleStatusBar->setChecked(true);
        actionAbout = new QAction(SerialToolClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(SerialToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 1, 1, 1);
        portConfigWidget = new QWidget(centralWidget);
        portConfigWidget->setObjectName(QStringLiteral("portConfigWidget"));
        portConfigWidget->setEnabled(true);
        horizontalLayout = new QHBoxLayout(portConfigWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 0, 2, 0);
        label = new QLabel(portConfigWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(label);

        comboBoxPortNum = new QComboBox(portConfigWidget);
        comboBoxPortNum->setObjectName(QStringLiteral("comboBoxPortNum"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(comboBoxPortNum->sizePolicy().hasHeightForWidth());
        comboBoxPortNum->setSizePolicy(sizePolicy3);
        comboBoxPortNum->setMinimumSize(QSize(120, 0));
        comboBoxPortNum->setMaximumSize(QSize(120, 16777215));
        comboBoxPortNum->setStyleSheet(QStringLiteral("width: 120px;"));

        horizontalLayout->addWidget(comboBoxPortNum);

        label_2 = new QLabel(portConfigWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(label_2);

        comboBoxBaudRate = new QComboBox(portConfigWidget);
        comboBoxBaudRate->setObjectName(QStringLiteral("comboBoxBaudRate"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(comboBoxBaudRate->sizePolicy().hasHeightForWidth());
        comboBoxBaudRate->setSizePolicy(sizePolicy4);

        horizontalLayout->addWidget(comboBoxBaudRate);


        verticalLayout_2->addWidget(portConfigWidget);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        textEditRead = new QTextEdit(tab_2);
        textEditRead->setObjectName(QStringLiteral("textEditRead"));
        textEditRead->setReadOnly(true);

        verticalLayout_3->addWidget(textEditRead);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        widget = new QWidget(tab_2);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy2.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy2);
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(3);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(4, 4, 2, 0);
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(5);
        gridLayout_2->setContentsMargins(-1, -1, -1, 4);
        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setEnabled(false);

        gridLayout_2->addWidget(checkBox_2, 1, 0, 1, 1);

        portReadHex = new QRadioButton(groupBox);
        portReadHex->setObjectName(QStringLiteral("portReadHex"));

        gridLayout_2->addWidget(portReadHex, 0, 1, 1, 1);

        portReadAscii = new QRadioButton(groupBox);
        portReadAscii->setObjectName(QStringLiteral("portReadAscii"));
        portReadAscii->setChecked(true);

        gridLayout_2->addWidget(portReadAscii, 0, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy3.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy3);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(5);
        gridLayout->setContentsMargins(-1, -1, 4, 4);
        portWriteHex = new QRadioButton(groupBox_2);
        portWriteHex->setObjectName(QStringLiteral("portWriteHex"));

        gridLayout->addWidget(portWriteHex, 1, 1, 1, 1);

        resendBox = new QCheckBox(groupBox_2);
        resendBox->setObjectName(QStringLiteral("resendBox"));

        gridLayout->addWidget(resendBox, 2, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 3, 1, 1);

        spinBoxStepTime = new QSpinBox(groupBox_2);
        spinBoxStepTime->setObjectName(QStringLiteral("spinBoxStepTime"));
        spinBoxStepTime->setMinimum(10);
        spinBoxStepTime->setMaximum(120000);
        spinBoxStepTime->setValue(1000);

        gridLayout->addWidget(spinBoxStepTime, 2, 1, 1, 1);

        portWriteAscii = new QRadioButton(groupBox_2);
        portWriteAscii->setObjectName(QStringLiteral("portWriteAscii"));
        portWriteAscii->setChecked(true);

        gridLayout->addWidget(portWriteAscii, 1, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox_2);


        horizontalLayout_5->addWidget(widget);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        sendButton = new QPushButton(tab_2);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setEnabled(false);
        sizePolicy4.setHeightForWidth(sendButton->sizePolicy().hasHeightForWidth());
        sendButton->setSizePolicy(sizePolicy4);

        gridLayout_3->addWidget(sendButton, 1, 1, 1, 1);

        textEditWrite = new QTextEdit(tab_2);
        textEditWrite->setObjectName(QStringLiteral("textEditWrite"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(textEditWrite->sizePolicy().hasHeightForWidth());
        textEditWrite->setSizePolicy(sizePolicy5);

        gridLayout_3->addWidget(textEditWrite, 0, 0, 1, 2);

        comboBox = new QComboBox(tab_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout_3->addWidget(comboBox, 1, 0, 1, 1);


        horizontalLayout_5->addLayout(gridLayout_3);


        verticalLayout_3->addLayout(horizontalLayout_5);

        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        customPlot = new QCustomPlot(tab);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        sizePolicy1.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy1);
        customPlot->setMinimumSize(QSize(200, 100));

        verticalLayout->addWidget(customPlot);

        horizontalScrollBar = new QScrollBar(tab);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(horizontalScrollBar->sizePolicy().hasHeightForWidth());
        horizontalScrollBar->setSizePolicy(sizePolicy6);
        horizontalScrollBar->setPageStep(100);
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalScrollBar);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        channelList = new QListWidget(tab);
        channelList->setObjectName(QStringLiteral("channelList"));
        QSizePolicy sizePolicy7(QSizePolicy::Ignored, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(channelList->sizePolicy().hasHeightForWidth());
        channelList->setSizePolicy(sizePolicy7);

        verticalLayout_9->addWidget(channelList);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_9->addWidget(label_4);

        yRateUpperBox = new QDoubleSpinBox(tab);
        yRateUpperBox->setObjectName(QStringLiteral("yRateUpperBox"));
        yRateUpperBox->setMinimum(-100000);
        yRateUpperBox->setMaximum(100000);

        verticalLayout_9->addWidget(yRateUpperBox);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_9->addWidget(label_5);

        yRateLowerBox = new QDoubleSpinBox(tab);
        yRateLowerBox->setObjectName(QStringLiteral("yRateLowerBox"));
        yRateLowerBox->setMinimum(-100000);

        verticalLayout_9->addWidget(yRateLowerBox);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_9->addWidget(label_6);

        xRangeBox = new QComboBox(tab);
        xRangeBox->setObjectName(QStringLiteral("xRangeBox"));
        xRangeBox->setMaxVisibleItems(20);

        verticalLayout_9->addWidget(xRangeBox);

        holdRxOscBox = new QCheckBox(tab);
        holdRxOscBox->setObjectName(QStringLiteral("holdRxOscBox"));
        holdRxOscBox->setChecked(true);

        verticalLayout_9->addWidget(holdRxOscBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_9);

        tabWidget->addTab(tab, QString());

        verticalLayout_2->addWidget(tabWidget);

        SerialToolClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SerialToolClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QStringLiteral("menu_5"));
        SerialToolClass->setMenuBar(menuBar);
        toolBar1 = new QToolBar(SerialToolClass);
        toolBar1->setObjectName(QStringLiteral("toolBar1"));
        toolBar1->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
        SerialToolClass->addToolBar(Qt::TopToolBarArea, toolBar1);
        statusBar = new QStatusBar(SerialToolClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SerialToolClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menu->addAction(actionSave);
        menu->addSeparator();
        menu->addAction(actionClose);
        menu_2->addAction(actionOption);
        menu_3->addAction(openPortAction);
        menu_3->addAction(portRunAction);
        menu_3->addAction(clearAction);
        menu_3->addSeparator();
        menu_3->addAction(portSetAction);
        menu_4->addAction(actionVisibleTab0);
        menu_4->addAction(actionVisibleTab1);
        menu_4->addSeparator();
        menu_4->addAction(actionVisibleToolbar);
        menu_4->addAction(actionVisibleStatusBar);
        menu_5->addAction(actionAbout);
        toolBar1->addAction(portSetAction);
        toolBar1->addSeparator();
        toolBar1->addAction(openPortAction);
        toolBar1->addAction(portRunAction);
        toolBar1->addAction(clearAction);
        toolBar1->addSeparator();
        toolBar1->addAction(actionOption);

        retranslateUi(SerialToolClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SerialToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *SerialToolClass)
    {
        SerialToolClass->setWindowTitle(QApplication::translate("SerialToolClass", "SerialTool", 0));
        actionSave->setText(QApplication::translate("SerialToolClass", "\344\277\235\345\255\230", 0));
        actionSave->setShortcut(QApplication::translate("SerialToolClass", "Ctrl+S", 0));
        actionOption->setText(QApplication::translate("SerialToolClass", "\351\200\211\351\241\271", 0));
        openPortAction->setText(QApplication::translate("SerialToolClass", "\346\211\223\345\274\200\347\253\257\345\217\243", 0));
        openPortAction->setShortcut(QApplication::translate("SerialToolClass", "Alt+S", 0));
        portRunAction->setText(QApplication::translate("SerialToolClass", "\346\232\202\345\201\234\345\217\221\351\200\201/\346\216\245\346\224\266", 0));
        portRunAction->setShortcut(QApplication::translate("SerialToolClass", "Alt+R", 0));
        clearAction->setText(QApplication::translate("SerialToolClass", "\346\270\205\347\251\272\346\225\260\346\215\256", 0));
        clearAction->setShortcut(QApplication::translate("SerialToolClass", "Alt+C", 0));
        portSetAction->setText(QApplication::translate("SerialToolClass", "\347\253\257\345\217\243\345\217\202\346\225\260\350\256\276\347\275\256", 0));
#ifndef QT_NO_TOOLTIP
        portSetAction->setToolTip(QApplication::translate("SerialToolClass", "\347\253\257\345\217\243\350\256\276\347\275\256", 0));
#endif // QT_NO_TOOLTIP
        actionClose->setText(QApplication::translate("SerialToolClass", "\351\200\200\345\207\272", 0));
        actionVisibleToolbar->setText(QApplication::translate("SerialToolClass", "\345\267\245\345\205\267\346\240\217", 0));
        actionVisibleTab0->setText(QApplication::translate("SerialToolClass", "\344\270\262\345\217\243\350\260\203\350\257\225\345\212\251\346\211\213", 0));
#ifndef QT_NO_TOOLTIP
        actionVisibleTab0->setToolTip(QApplication::translate("SerialToolClass", "\346\230\276\347\244\272\344\270\262\345\217\243\350\260\203\350\257\225\345\212\251\346\211\213", 0));
#endif // QT_NO_TOOLTIP
        actionVisibleTab1->setText(QApplication::translate("SerialToolClass", "\344\270\262\345\217\243\347\244\272\346\263\242\345\231\250", 0));
#ifndef QT_NO_TOOLTIP
        actionVisibleTab1->setToolTip(QApplication::translate("SerialToolClass", "\344\270\262\345\217\243\347\244\272\346\263\242\345\231\250", 0));
#endif // QT_NO_TOOLTIP
        actionVisibleStatusBar->setText(QApplication::translate("SerialToolClass", "\347\212\266\346\200\201\346\240\217", 0));
        actionAbout->setText(QApplication::translate("SerialToolClass", "\345\205\263\344\272\216", 0));
        actionAbout->setShortcut(QApplication::translate("SerialToolClass", "F1", 0));
        label->setText(QApplication::translate("SerialToolClass", "\347\253\257\345\217\243\357\274\232", 0));
        label_2->setText(QApplication::translate("SerialToolClass", "\346\263\242\347\211\271\347\216\207\357\274\232", 0));
        comboBoxBaudRate->clear();
        comboBoxBaudRate->insertItems(0, QStringList()
         << QApplication::translate("SerialToolClass", "1200", 0)
         << QApplication::translate("SerialToolClass", "2400", 0)
         << QApplication::translate("SerialToolClass", "4800", 0)
         << QApplication::translate("SerialToolClass", "9600", 0)
         << QApplication::translate("SerialToolClass", "19200", 0)
         << QApplication::translate("SerialToolClass", "38400", 0)
         << QApplication::translate("SerialToolClass", "57600", 0)
         << QApplication::translate("SerialToolClass", "115200", 0)
        );
        comboBoxBaudRate->setCurrentText(QApplication::translate("SerialToolClass", "1200", 0));
        groupBox->setTitle(QApplication::translate("SerialToolClass", "\346\216\245\346\224\266\350\256\276\347\275\256", 0));
        checkBox_2->setText(QApplication::translate("SerialToolClass", "\350\207\252\345\212\250\346\215\242\350\241\214", 0));
        portReadHex->setText(QApplication::translate("SerialToolClass", "Hex", 0));
        portReadAscii->setText(QApplication::translate("SerialToolClass", "ASCII", 0));
        groupBox_2->setTitle(QApplication::translate("SerialToolClass", "\345\217\221\351\200\201\350\256\276\347\275\256", 0));
        portWriteHex->setText(QApplication::translate("SerialToolClass", "Hex", 0));
        resendBox->setText(QApplication::translate("SerialToolClass", "\351\207\215\345\244\215\345\217\221\351\200\201", 0));
        label_3->setText(QApplication::translate("SerialToolClass", "ms", 0));
        portWriteAscii->setText(QApplication::translate("SerialToolClass", "ASCII", 0));
#ifndef QT_NO_TOOLTIP
        sendButton->setToolTip(QApplication::translate("SerialToolClass", "Ctrl + Enter", 0));
#endif // QT_NO_TOOLTIP
        sendButton->setText(QApplication::translate("SerialToolClass", "\345\217\221\351\200\201", 0));
        sendButton->setShortcut(QApplication::translate("SerialToolClass", "Ctrl+Return", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SerialToolClass", "\344\270\262\345\217\243\350\260\203\350\257\225\345\212\251\346\211\213", 0));
        label_4->setText(QApplication::translate("SerialToolClass", "Y\350\275\264\344\270\212\351\231\220\357\274\232", 0));
        label_5->setText(QApplication::translate("SerialToolClass", "Y\350\275\264\344\270\213\351\231\220\357\274\232", 0));
        label_6->setText(QApplication::translate("SerialToolClass", "X\350\275\264\351\207\207\346\240\267\346\225\260\357\274\232", 0));
        xRangeBox->clear();
        xRangeBox->insertItems(0, QStringList()
         << QApplication::translate("SerialToolClass", "10", 0)
         << QApplication::translate("SerialToolClass", "20", 0)
         << QApplication::translate("SerialToolClass", "25", 0)
         << QApplication::translate("SerialToolClass", "50", 0)
         << QApplication::translate("SerialToolClass", "100", 0)
         << QApplication::translate("SerialToolClass", "200", 0)
         << QApplication::translate("SerialToolClass", "250", 0)
         << QApplication::translate("SerialToolClass", "500", 0)
         << QApplication::translate("SerialToolClass", "1000", 0)
         << QApplication::translate("SerialToolClass", "2000", 0)
         << QApplication::translate("SerialToolClass", "2500", 0)
         << QApplication::translate("SerialToolClass", "5000", 0)
         << QApplication::translate("SerialToolClass", "10000", 0)
        );
        xRangeBox->setCurrentText(QApplication::translate("SerialToolClass", "10", 0));
#ifndef QT_NO_TOOLTIP
        holdRxOscBox->setToolTip(QApplication::translate("SerialToolClass", "\344\270\262\345\217\243\347\244\272\346\263\242\345\231\250\345\234\250\345\205\266\344\273\226\351\200\211\351\241\271\345\215\241\344\270\213\344\271\237\344\274\232\346\216\245\346\224\266\346\225\260\346\215\256", 0));
#endif // QT_NO_TOOLTIP
        holdRxOscBox->setText(QApplication::translate("SerialToolClass", "\344\277\235\346\214\201\346\216\245\346\224\266", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SerialToolClass", "\344\270\262\345\217\243\347\244\272\346\263\242\345\231\250", 0));
        menu->setTitle(QApplication::translate("SerialToolClass", "\346\226\207\344\273\266", 0));
        menu_2->setTitle(QApplication::translate("SerialToolClass", "\350\256\276\347\275\256", 0));
        menu_3->setTitle(QApplication::translate("SerialToolClass", "\347\274\226\350\276\221", 0));
        menu_4->setTitle(QApplication::translate("SerialToolClass", "\350\247\206\345\233\276", 0));
        menu_5->setTitle(QApplication::translate("SerialToolClass", "\345\270\256\345\212\251", 0));
        toolBar1->setWindowTitle(QApplication::translate("SerialToolClass", "\345\267\245\345\205\267\346\240\217", 0));
    } // retranslateUi

};

namespace Ui {
    class SerialToolClass: public Ui_SerialToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALTOOL_H
