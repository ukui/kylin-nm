QT += testlib dbus widgets core network
QT -= gui

CONFIG += qt console warn_on thread
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_proxyapphandler

INCLUDEPATH += ../../../../sys-dbus-register
INCLUDEPATH += ../../../../sys-dbus-register/proxyServer
INCLUDEPATH += ../../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/libnm
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include

DEPENDPATH += ../../../../sys-dbus-register
DEPENDPATH += ../../../../sys-dbus-register/proxyServer
DEPENDPATH += ../../../../common

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libcap

LIBS += -L/usr/lib/ -lKF5NetworkManagerQt -lcap

SOURCES += test_proxyapphandler.cpp
SOURCES += ../../../sys-dbus-register/proxyServer/proxyapphandler.cpp

HEADERS += test_proxyapphandler.h
HEADERS += ../../../sys-dbus-register/proxyServer/proxyapphandler.h
HEADERS += ../../../sys-dbus-register/authority-base.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_NO_DEBUG

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
