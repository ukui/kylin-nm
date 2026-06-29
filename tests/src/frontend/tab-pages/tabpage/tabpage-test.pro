QT += core gui widgets dbus testlib network svg x11extras

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_tabpage

INCLUDEPATH += ../../../../../../src/frontend
INCLUDEPATH += ../../../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../../src/frontend/enterprise-wlan
INCLUDEPATH += ../../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../../src/frontend/connectivity
INCLUDEPATH += ../../../../../../src/frontend/xatom
INCLUDEPATH += ../../../../../../src/frontend/list-items
INCLUDEPATH += ../../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../../src
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

SOURCES += ../../../../../../src/frontend/tab-pages/tabpage.cpp
SOURCES += ../../../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../../../src/frontend/tools/switchbutton.cpp
SOURCES += ../../../../../../src/frontend/tools/divider.cpp
SOURCES += ../../../../../../src/frontend/tools/radioitembutton.cpp
SOURCES += ../../../../../../src/frontend/tools/infobutton.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectresource.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += $$PWD/test_tabpage.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../../../src/frontend/tab-pages/tabpage.h
HEADERS += ../../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../../src/frontend/tools/switchbutton.h
HEADERS += ../../../../../../src/frontend/tools/divider.h
HEADERS += ../../../../../../src/frontend/tools/radioitembutton.h
HEADERS += ../../../../../../src/frontend/tools/infobutton.h
HEADERS += $$PWD/test_tabpage.h