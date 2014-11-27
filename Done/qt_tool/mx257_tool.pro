#-------------------------------------------------
#
# Project created by QtCreator 2014-08-26T09:39:40
#
#-------------------------------------------------

QT       += core gui\
        network

TARGET = mx257_tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialbase.cpp \
    posix_qextserialport.cpp \
    tcpserverdialog.cpp \
    tcpclientdialog.cpp \
    serialdialog.cpp \
    tooltab.cpp

HEADERS  += mainwindow.h \
    qextserialbase.h \
    posix_qextserialport.h \
    tcpserverdialog.h \
    tcpclientdialog.h \
    serialdialog.h \
    tooltab.h

FORMS    += mainwindow.ui \
    tcpserverdialog.ui \
    tcpclientdialog.ui \
    serialdialog.ui

OTHER_FILES += \
    src/README
