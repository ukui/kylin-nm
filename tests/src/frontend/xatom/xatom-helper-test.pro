QT += core gui widgets dbus testlib x11extras

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_xatom-helper

INCLUDEPATH += $$PWD/../../../../../src/frontend
INCLUDEPATH += $$PWD/../../../../../src/frontend/xatom
INCLUDEPATH += $$PWD/../../../../../src
INCLUDEPATH += $$PWD/../../../../../common
INCLUDEPATH += $$PWD/../../common

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/KF5/KWindowSystem
INCLUDEPATH += /usr/include/kysdk/applications

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS
QMAKE_LIBS += -lX11

SOURCES += ../../../../../src/frontend/xatom/xatom-helper.cpp
SOURCES += $$PWD/test_xatom-helper.cpp

HEADERS += ../../../../../src/frontend/xatom/xatom-helper.h
HEADERS += $$PWD/test_xatom-helper.h
