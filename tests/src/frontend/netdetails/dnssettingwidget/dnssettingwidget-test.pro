QT += core gui widgets dbus testlib network

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_dnssettingwidget

INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../common
INCLUDEPATH += ../../../../../pub
INCLUDEPATH += ../../../../../src/backend
INCLUDEPATH += ../../../../../src/backend/dbus-interface

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QGSettings

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

SOURCES += ../../../../../src/frontend/netdetails/dnssettingwidget.cpp
SOURCES += ../../../../../src/frontend/tools/divider.cpp
SOURCES += test_dnssettingwidget.cpp
SOURCES += main.cpp

HEADERS += ../../../../../src/frontend/netdetails/dnssettingwidget.h
HEADERS += ../../../../../src/frontend/tools/divider.h
HEADERS += ../../../../../src/frontend/netdetails/coninfo.h
HEADERS += test_dnssettingwidget.h
