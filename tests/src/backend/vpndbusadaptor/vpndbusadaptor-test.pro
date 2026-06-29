QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_vpndbusadaptor

INCLUDEPATH += ../../../../src/backend
INCLUDEPATH += ../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../src/frontend/tools
INCLUDEPATH += ../../../../src/frontend
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../../src-vpn/frontend
INCLUDEPATH += ../../../../src-vpn/backend
INCLUDEPATH += ../../../../src-vpn/frontend/single-pages

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt

SOURCES +=     ../../../../src/backend/vpndbusadaptor.cpp     $$PWD/test_vpndbusadaptor.cpp     $$PWD/main.cpp

HEADERS +=     ../../../../src/backend/vpndbusadaptor.h     $$PWD/test_vpndbusadaptor.h
