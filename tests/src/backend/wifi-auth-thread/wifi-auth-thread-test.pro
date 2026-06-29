QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_wifi-auth-thread

INCLUDEPATH += ../../../../src/backend
INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../common

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

SOURCES +=     ../../../../src/backend/wifi-auth-thread.cpp     $$PWD/test_wifi-auth-thread.cpp     $$PWD/main.cpp

HEADERS +=     ../../../../src/backend/wifi-auth-thread.h     $$PWD/test_wifi-auth-thread.h
