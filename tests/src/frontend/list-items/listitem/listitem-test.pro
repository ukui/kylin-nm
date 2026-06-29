QT += core gui widgets dbus testlib network svg

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_listitem

INCLUDEPATH += ../../../../../../src/frontend
INCLUDEPATH += ../../../../../../src/frontend/list-items
INCLUDEPATH += ../../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../../src/frontend/enterprise-wlan
INCLUDEPATH += ../../../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../../src/frontend/connectivity
INCLUDEPATH += ../../../../../../src/frontend/xatom
INCLUDEPATH += ../../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../../src
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../common

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

SOURCES += ../../../../../../src/frontend/list-items/listitem.cpp
SOURCES += ../../../../../../src/frontend/tools/radioitembutton.cpp
SOURCES += ../../../../../../src/frontend/tools/infobutton.cpp
SOURCES += ../../../../../../src/frontend/tools/kylable.cpp
SOURCES += $$PWD/test_listitem.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../../../src/frontend/list-items/listitem.h
HEADERS += ../../../../../../src/frontend/tools/radioitembutton.h
HEADERS += ../../../../../../src/frontend/tools/infobutton.h
HEADERS += ../../../../../../src/frontend/tools/kylable.h
HEADERS += $$PWD/test_listitem.h

