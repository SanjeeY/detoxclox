#-------------------------------------------------
#
# Project created by QtCreator 2013-10-06T13:36:59
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += static
TARGET = detoxCalc
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    setperioddialog.cpp

HEADERS  += mainwindow.h \
    setperioddialog.h

FORMS    += mainwindow.ui \
    setperioddialog.ui

RESOURCES += \
    desktopIcon.qrc
