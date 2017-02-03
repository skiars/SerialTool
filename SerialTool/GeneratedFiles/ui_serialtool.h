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

class Ui_SerialTool
{
public:
    QAction *actionSave;
    QAction *actionOption;
    QAction *portSwitchAction;
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
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *portReadAscii;
    QRadioButton *portReadHex;
    QCheckBox *checkBox_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *portWriteAscii;
    QRadioButton *portWriteHex;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *resendBox;
    QSpinBox *spinBoxStepTime;
    QLabel *label_3;
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

    void setupUi(QMainWindow *SerialTool)
    {
        if (SerialTool->objectName().isEmpty())
            SerialTool->setObjectName(QStringLiteral("SerialTool"));
        SerialTool->resize(600, 426);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SerialTool->sizePolicy().hasHeightForWidth());
        SerialTool->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/SerialTool/images/icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SerialTool->setWindowIcon(icon);
        actionSave = new QAction(SerialTool);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SerialTool/images/save.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionOption = new QAction(SerialTool);
        actionOption->setObjectName(QStringLiteral("actionOption"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/SerialTool/images/config.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionOption->setIcon(icon2);
        portSwitchAction = new QAction(SerialTool);
        portSwitchAction->setObjectName(QStringLiteral("portSwitchAction"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/SerialTool/images/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        portSwitchAction->setIcon(icon3);
        portRunAction = new QAction(SerialTool);
        portRunAction->setObjectName(QStringLiteral("portRunAction"));
        portRunAction->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/SerialTool/images/pause.ico"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QStringLiteral(":/SerialTool/images/pause.png"), QSize(), QIcon::Active, QIcon::Off);
        portRunAction->setIcon(icon4);
        clearAction = new QAction(SerialTool);
        clearAction->setObjectName(QStringLiteral("clearAction"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/SerialTool/images/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearAction->setIcon(icon5);
        portSetAction = new QAction(SerialTool);
        portSetAction->setObjectName(QStringLiteral("portSetAction"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/SerialTool/images/port config.png"), QSize(), QIcon::Normal, QIcon::Off);
        portSetAction->setIcon(icon6);
        actionClose = new QAction(SerialTool);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/SerialTool/images/exit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon7);
        actionVisibleToolbar = new QAction(SerialTool);
        actionVisibleToolbar->setObjectName(QStringLiteral("actionVisibleToolbar"));
        actionVisibleToolbar->setCheckable(true);
        actionVisibleToolbar->setChecked(true);
        actionVisibleTab0 = new QAction(SerialTool);
        actionVisibleTab0->setObjectName(QStringLiteral("actionVisibleTab0"));
        actionVisibleTab0->setCheckable(true);
        actionVisibleTab1 = new QAction(SerialTool);
        actionVisibleTab1->setObjectName(QStringLiteral("actionVisibleTab1"));
        actionVisibleTab1->setCheckable(true);
        actionVisibleStatusBar = new QAction(SerialTool);
        actionVisibleStatusBar->setObjectName(QStringLiteral("actionVisibleStatusBar"));
        actionVisibleStatusBar->setCheckable(true);
        actionVisibleStatusBar->setChecked(true);
        actionAbout = new QAction(SerialTool);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(SerialTool);
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
        comboBoxBaudRate->insertItems(0, QStringList()
         << QStringLiteral("1200")
         << QStringLiteral("2400")
         << QStringLiteral("4800")
         << QStringLiteral("9600")
         << QStringLiteral("19200")
         << QStringLiteral("38400")
         << QStringLiteral("57600")
         << QStringLiteral("115200")
        );
        comboBoxBaudRate->setObjectName(QStringLiteral("comboBoxBaudRate"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(comboBoxBaudRate->sizePolicy().hasHeightForWidth());
        comboBoxBaudRate->setSizePolicy(sizePolicy4);
        comboBoxBaudRate->setCurrentText(QStringLiteral("1200"));

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
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setSpacing(2);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(4, -1, 4, 4);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        portReadAscii = new QRadioButton(groupBox);
        portReadAscii->setObjectName(QStringLiteral("portReadAscii"));
        portReadAscii->setText(QStringLiteral("ASCII"));
        portReadAscii->setChecked(true);

        horizontalLayout_3->addWidget(portReadAscii);

        portReadHex = new QRadioButton(groupBox);
        portReadHex->setObjectName(QStringLiteral("portReadHex"));
        portReadHex->setText(QStringLiteral("Hex"));

        horizontalLayout_3->addWidget(portReadHex);


        verticalLayout_5->addLayout(horizontalLayout_3);

        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setEnabled(false);

        verticalLayout_5->addWidget(checkBox_2);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy3.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy3);
        verticalLayout_6 = new QVBoxLayout(groupBox_2);
        verticalLayout_6->setSpacing(2);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(4, -1, 4, 4);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        portWriteAscii = new QRadioButton(groupBox_2);
        portWriteAscii->setObjectName(QStringLiteral("portWriteAscii"));
        portWriteAscii->setText(QStringLiteral("ASCII"));
        portWriteAscii->setChecked(true);

        horizontalLayout_4->addWidget(portWriteAscii);

        portWriteHex = new QRadioButton(groupBox_2);
        portWriteHex->setObjectName(QStringLiteral("portWriteHex"));
        portWriteHex->setText(QStringLiteral("Hex"));

        horizontalLayout_4->addWidget(portWriteHex);


        verticalLayout_6->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(2);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        resendBox = new QCheckBox(groupBox_2);
        resendBox->setObjectName(QStringLiteral("resendBox"));

        horizontalLayout_6->addWidget(resendBox);

        spinBoxStepTime = new QSpinBox(groupBox_2);
        spinBoxStepTime->setObjectName(QStringLiteral("spinBoxStepTime"));
        spinBoxStepTime->setMinimum(10);
        spinBoxStepTime->setMaximum(120000);
        spinBoxStepTime->setValue(1000);

        horizontalLayout_6->addWidget(spinBoxStepTime);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);


        verticalLayout_6->addLayout(horizontalLayout_6);


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
        xRangeBox->insertItems(0, QStringList()
         << QStringLiteral("10")
         << QStringLiteral("20")
         << QStringLiteral("25")
         << QStringLiteral("50")
         << QStringLiteral("100")
         << QStringLiteral("200")
         << QStringLiteral("250")
         << QStringLiteral("500")
         << QStringLiteral("1000")
         << QStringLiteral("2000")
         << QStringLiteral("2500")
         << QStringLiteral("5000")
         << QStringLiteral("10000")
        );
        xRangeBox->setObjectName(QStringLiteral("xRangeBox"));
        xRangeBox->setCurrentText(QStringLiteral("10"));
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

        SerialTool->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SerialTool);
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
        SerialTool->setMenuBar(menuBar);
        toolBar1 = new QToolBar(SerialTool);
        toolBar1->setObjectName(QStringLiteral("toolBar1"));
        toolBar1->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
        SerialTool->addToolBar(Qt::TopToolBarArea, toolBar1);
        statusBar = new QStatusBar(SerialTool);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SerialTool->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menu->addAction(actionSave);
        menu->addSeparator();
        menu->addAction(actionClose);
        menu_2->addAction(actionOption);
        menu_3->addAction(portSwitchAction);
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
        toolBar1->addAction(portSwitchAction);
        toolBar1->addAction(portRunAction);
        toolBar1->addAction(clearAction);
        toolBar1->addSeparator();
        toolBar1->addAction(actionOption);

        retranslateUi(SerialTool);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SerialTool);
    } // setupUi

    void retranslateUi(QMainWindow *SerialTool)
    {
        SerialTool->setWindowTitle(QApplication::translate("SerialTool", "SerialTool", 0));
        actionSave->setText(QApplication::translate("SerialTool", "Save", 0));
        actionSave->setShortcut(QApplication::translate("SerialTool", "Ctrl+S", 0));
        actionOption->setText(QApplication::translate("SerialTool", "Options", 0));
        portSwitchAction->setText(QApplication::translate("SerialTool", "Open Port", 0));
        portSwitchAction->setShortcut(QApplication::translate("SerialTool", "Alt+S", 0));
        portRunAction->setText(QApplication::translate("SerialTool", "Pause Tx/Rx", 0));
        portRunAction->setShortcut(QApplication::translate("SerialTool", "Alt+R", 0));
        clearAction->setText(QApplication::translate("SerialTool", "Clear Buffer", 0));
        clearAction->setShortcut(QApplication::translate("SerialTool", "Alt+C", 0));
        portSetAction->setText(QApplication::translate("SerialTool", "Port Settings", 0));
        actionClose->setText(QApplication::translate("SerialTool", "Close", 0));
        actionVisibleToolbar->setText(QApplication::translate("SerialTool", "Tool Bar", 0));
        actionVisibleTab0->setText(QApplication::translate("SerialTool", "Tester", 0));
#ifndef QT_NO_TOOLTIP
        actionVisibleTab0->setToolTip(QApplication::translate("SerialTool", "Tester", 0));
#endif // QT_NO_TOOLTIP
        actionVisibleTab1->setText(QApplication::translate("SerialTool", "Plot", 0));
        actionVisibleStatusBar->setText(QApplication::translate("SerialTool", "Status Bar", 0));
        actionAbout->setText(QApplication::translate("SerialTool", "About", 0));
        actionAbout->setShortcut(QApplication::translate("SerialTool", "F1", 0));
        label->setText(QApplication::translate("SerialTool", "Port", 0));
        label_2->setText(QApplication::translate("SerialTool", "Baud Rate", 0));
        groupBox->setTitle(QApplication::translate("SerialTool", "Rx Settings", 0));
        checkBox_2->setText(QApplication::translate("SerialTool", "Auto Linefeed", 0));
        groupBox_2->setTitle(QApplication::translate("SerialTool", "Tx Settings", 0));
        resendBox->setText(QApplication::translate("SerialTool", "Resend", 0));
        label_3->setText(QApplication::translate("SerialTool", "ms", 0));
#ifndef QT_NO_TOOLTIP
        sendButton->setToolTip(QApplication::translate("SerialTool", "Ctrl+Enter", 0));
#endif // QT_NO_TOOLTIP
        sendButton->setText(QApplication::translate("SerialTool", "Send", 0));
        sendButton->setShortcut(QApplication::translate("SerialTool", "Ctrl+Return", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SerialTool", "Tester", 0));
        label_4->setText(QApplication::translate("SerialTool", "Y Axis Upper", 0));
        label_5->setText(QApplication::translate("SerialTool", "Y Axis Lower", 0));
        label_6->setText(QApplication::translate("SerialTool", "X Axis Points", 0));
        holdRxOscBox->setText(QApplication::translate("SerialTool", "Hold receive", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SerialTool", "Plot", 0));
        menu->setTitle(QApplication::translate("SerialTool", "File", 0));
        menu_2->setTitle(QApplication::translate("SerialTool", "Tools", 0));
        menu_3->setTitle(QApplication::translate("SerialTool", "Edit", 0));
        menu_4->setTitle(QApplication::translate("SerialTool", "View", 0));
        menu_5->setTitle(QApplication::translate("SerialTool", "Help", 0));
        toolBar1->setWindowTitle(QApplication::translate("SerialTool", "Tool Bar", 0));
    } // retranslateUi

};

namespace Ui {
    class SerialTool: public Ui_SerialTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALTOOL_H
