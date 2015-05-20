#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T11:52:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpticalExperimentWithLenses
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingitem.cpp \
    lensesetupwindow.cpp

HEADERS  += mainwindow.h \
    settingitem.h \
    lensesetupwindow.h

FORMS    += mainwindow.ui \
    settingitem.ui \
    lensesetupwindow.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    example.txt
