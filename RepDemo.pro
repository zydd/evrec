#-------------------------------------------------
#
# Project created by QtCreator 2014-09-18T17:10:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RepDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    event.cpp \
    eventrecorder.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    event.h \
    eventrecorder.h \
    dialog.h

FORMS    += \
    dialog.ui
