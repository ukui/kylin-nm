# Unit test project for src/main.cpp

QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt warn_on link_pkgconfig no_keywords
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_main

INCLUDEPATH += $$PWD/../../../src
INCLUDEPATH += $$PWD/../../../src/backend
INCLUDEPATH += $$PWD/../../../src/backend/dbus-interface
INCLUDEPATH += $$PWD/../../../../common

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

LIBS += -lgtest -lgtest_main -lKF5NetworkManagerQt
LIBS += -lX11

QMAKE_LFLAGS += --coverage
QMAKE_CXXFLAGS += --coverage

SOURCES += $$PWD/../../../src/main.cpp
SOURCES += $$PWD/../../../src/backend/dbus.cpp
SOURCES += test_main.cpp
SOURCES += test_main_entry.cpp

HEADERS += $$PWD/../../../src/backend/dbus.h
HEADERS += test_main.h