#-------------------------------------------------
#
# Project created by QtCreator 2018-06-10T15:50:19
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Robo_Hand
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        robohand.cpp \
    uisetting.cpp \
    movewidget.cpp \
    popmenu.cpp \
    mybut.cpp \
    widgetmenu.cpp \
    headleftwidget.cpp \
    leftsidemenu.cpp \
    popmenucommand.cpp \
    signalgraphics.cpp \
    comconnectwidget.cpp \
    comport.cpp \
    voicedata.cpp \
    handlersignal.cpp \
    recognizer.cpp

HEADERS += \
        robohand.h \
    uisetting.h \
    movewidget.h \
    popmenu.h \
    mybut.h \
    widgetmenu.h \
    headleftwidget.h \
    leftsidemenu.h \
    popmenucommand.h \
    signalgraphics.h \
    comconnectwidget.h \
    comport.h \
    voicedata.h \
    handlersignal.h \
    recognizer.h

FORMS += \
        robohand.ui

RESOURCES += \
    ../img/img.qrc \
    ../img/img.qrc

DISTFILES +=
