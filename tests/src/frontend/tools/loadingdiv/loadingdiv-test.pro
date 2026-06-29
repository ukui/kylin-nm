QT += core gui widgets dbus testlib

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_loadingdiv

INCLUDEPATH += $$PWD/../../../../../src/frontend
INCLUDEPATH += $$PWD/../../../../../src/frontend/tab-pages
INCLUDEPATH += $$PWD/../../../../../src/frontend/tools
INCLUDEPATH += $$PWD/../../../../../src/frontend/netdetails
INCLUDEPATH += $$PWD/../../../../../src/frontend/enterprise-wlan
INCLUDEPATH += $$PWD/../../../../../src/frontend/connectivity
INCLUDEPATH += $$PWD/../../../../../src/frontend/networkmode
INCLUDEPATH += $$PWD/../../../../../src/frontend/xatom
INCLUDEPATH += $$PWD/../../../../../src/frontend/list-items
INCLUDEPATH += $$PWD/../../../../../src
INCLUDEPATH += $$PWD/../../../../../common

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

SOURCES += $$PWD/../../../../../src/frontend/tools/loadingdiv.cpp
SOURCES += $$PWD/test_loadingdiv.cpp
SOURCES += $$PWD/main.cpp

HEADERS += $$PWD/../../../../../src/frontend/tools/loadingdiv.h
HEADERS += $$PWD/test_loadingdiv.h
