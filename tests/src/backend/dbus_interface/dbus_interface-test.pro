QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt no_keywords

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_dbus_interface

INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../../src/backend

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt

SOURCES += ../../../../src/backend/dbus_interface.cpp
SOURCES += test_dbus_interface.cpp
SOURCES += main.cpp

HEADERS += ../../../../src/backend/dbus_interface.h
HEADERS += test_dbus_interface.h
