QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt
CONFIG -= app_bundle
CONFIG += link_pkgconfig no_keywords

TEMPLATE = app

TARGET = test_kylinvpnconnectitem

INCLUDEPATH += $$PWD/../../../../../src/backend/dbus-interface
INCLUDEPATH += $$PWD/../../../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/include/glib-2.0

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap dbus-1

DEFINES += QT_DEPRECATED_WARNINGS


QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt
LIBS += -ldbus-1

SOURCES += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp
SOURCES += test_kylinvpnconnectitem.cpp
SOURCES += main.cpp

HEADERS += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.h
HEADERS += test_kylinvpnconnectitem.h

