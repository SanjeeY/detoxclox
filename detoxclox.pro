#-------------------------------------------------
#
# Project created by QtCreator 2013-10-06T13:36:59
#
#-------------------------------------------------

QT       += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += static
RC_FILE = appicon.rc
TARGET = detoxClox
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
