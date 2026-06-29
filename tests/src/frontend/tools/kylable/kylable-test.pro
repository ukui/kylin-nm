QT += core gui widgets dbus testlib

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kylable

INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../src/frontend/enterprise-wlan
INCLUDEPATH += ../../../../../src/frontend/connectivity
INCLUDEPATH += ../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../src/frontend/xatom
INCLUDEPATH += ../../../../../src/frontend/list-items
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../common

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

SOURCES += ../../../../../src/frontend/tools/kylable.cpp \
           test_kylable.cpp \
           main.cpp

HEADERS += ../../../../../src/frontend/tools/kylable.h \
           ../../../../../src/frontend/tools/themepalette.h \
           test_kylable.h
