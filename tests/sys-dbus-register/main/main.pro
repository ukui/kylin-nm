QT += dbus testlib network
QT -= gui

CONFIG += qt console c++14 link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_main

INCLUDEPATH += ../../../../sys-dbus-register

HEADERS += test_main.h

SOURCES += test_main.cpp main.cpp

LIBS += -L/usr/lib/ -lKF5NetworkManagerQt

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libcap

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
