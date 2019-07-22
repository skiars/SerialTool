#-------------------------------------------------
#
# Project created by QtCreator 2017-02-01T17:03:23
#
#-------------------------------------------------
QT       += core gui widgets serialport network charts script uitools

TARGET = SerialTool

TEMPLATE = app

CONFIG  += qscintilla2

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QSCINTILLA_DLL

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix {
    QMAKE_LFLAGS += -fno-pie -no-pie # create executable file
}

TRANSLATIONS += language/serialtool_zh_CN.ts

RC_FILE += resource/serialtool.rc

INCLUDEPATH += include

SOURCES += \
    src/aboutbox.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/controller.cpp \
    src/portsetbox.cpp \
    src/defaultconfig.cpp \
    src/updatedialog.cpp \
    src/port/portmanager.cpp \
    src/port/tcpudpport.cpp \
    src/port/serialport.cpp \
    src/views/viewmanager.cpp \
    src/views/scriptextension/scriptextensionview.cpp \
    src/views/texttr/textedit.cpp \
    src/views/texttr/texttrview.cpp \
    src/views/terminal/terminalview.cpp \
    src/views/terminal/qvterminal/qvtchar.cpp \
    src/views/terminal/qvterminal/qvtcharformat.cpp \
    src/views/terminal/qvterminal/qvterminal.cpp \
    src/views/terminal/qvterminal/qvtlayout.cpp \
    src/views/terminal/qvterminal/qvtline.cpp \
    src/views/oscilloscope/oscilloscopeview.cpp \
    src/views/oscilloscope/channelitem.cpp \
    src/views/oscilloscope/oscopetimestamp.cpp \
    src/views/oscilloscope/pointdatabuffer.cpp \
    src/views/oscilloscope/plotview.cpp \
    src/views/oscilloscope/wavedecode.cpp \
    src/views/filetransmit/filetransmitview.cpp \
    src/views/filetransmit/filethread.cpp \
    src/views/filetransmit/xmodem.cpp \
    src/toolbox/toolboxmanager.cpp \
    src/toolbox/vediobox/vediobox.cpp \
    src/toolbox/valuedisplay/valuedisplay.cpp \
    src/settings/optionsdialog.cpp \
    src/settings/settingswidget.cpp \
    src/settings/settingswidgets.cpp \
    src/settings/translate.cpp

HEADERS  += \
    include/aboutbox.h \
    include/mainwindow.h \
    include/controller.h \
    include/portsetbox.h \
    include/version.h \
    include/defaultconfig.h \
    include/updatedialog.h \
    src/port/portmanager.h \
    src/port/abstractport.h \
    src/port/tcpudpport.h \
    src/port/serialport.h \
    src/views/viewmanager.h \
    src/views/abstractview.h \
    src/views/scriptextension/scriptextensionview.h \
    src/views/texttr/textedit.h \
    src/views/texttr/texttrview.h \
    src/views/terminal/terminalview.h \
    src/views/terminal/qvterminal/qvtchar.h \
    src/views/terminal/qvterminal/qvtcharformat.h \
    src/views/terminal/qvterminal/qvterminal.h \
    src/views/terminal/qvterminal/qvtlayout.h \
    src/views/terminal/qvterminal/qvtline.h \
    src/views/oscilloscope/oscilloscopeview.h \
    src/views/oscilloscope/channelitem.h \
    src/views/oscilloscope/oscopetimestamp.h \
    src/views/oscilloscope/pointdatabuffer.h \
    src/views/oscilloscope/plotview.h \
    src/views/oscilloscope/wavedecode.h \
    src/views/filetransmit/filetransmitview.h \
    src/views/filetransmit/filethread.h \
    src/views/filetransmit/xmodem.h \
    src/views/filetransmit/xmodem_crc16.h \
    src/toolbox/toolboxmanager.h \
    src/toolbox/abstracttoolbox.h \
    src/toolbox/vediobox/vediobox.h \
    src/toolbox/valuedisplay/valuedisplay.h \
    src/settings/optionsdialog.h \
    src/settings/settingswidget.h \
    src/settings/settingswidgets.h \
    src/settings/translate.h

DISTFILES += \
    resource/images/clear.png \
    resource/images/close.png \
    resource/images/connect.png \
    resource/images/port config.png \
    resource/images/config.ico \
    resource/images/exit.ico \
    resource/images/logo.ico \
    resource/images/pause.ico \
    resource/images/save.ico \
    resource/images/start.ico

RESOURCES += \
    resource/serialtool.qrc

FORMS += \
    ui/aboutbox.ui \
    ui/portsetbox.ui \
    ui/vediobox.ui \
    ui/tcpudpport.ui \
    ui/valuedisplay.ui \
    ui/serialport.ui \
    ui/mainwindow.ui \
    ui/filetransmitview.ui \
    ui/updatedialog.ui \
    ui/oscilloscopeview.ui \
    ui/texttrview.ui
