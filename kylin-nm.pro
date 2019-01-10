#-------------------------------------------------
#
# Project created by QtCreator 2018-10-19T15:29:47
#
#-------------------------------------------------

QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kylin-nm

target.path = /usr/bin
target.source += $$TARGET
shells.path = /usr/share/kylin-nm/shell/
shells.files = connup.sh
desktop.path = /etc/xdg/autostart/
desktop.files = kylin-nm.desktop

INSTALLS += target \
    shells \
    desktop

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
        mainwindow.cpp \
        ksimplenm.cpp \
        oneconnform.cpp \
        confform.cpp \
        kylin-network-interface.c \
    backthread.cpp \
    onelancform.cpp \
    loadingdiv.cpp

HEADERS += \
        mainwindow.h \
        ksimplenm.h \
        oneconnform.h \
        confform.h \
        kylin-network-interface.h \
    backthread.h \
    onelancform.h \
    loadingdiv.h

FORMS += \
        mainwindow.ui \
        oneconnform.ui \
        confform.ui \
    onelancform.ui

RESOURCES += \
    nmqrc.qrc
