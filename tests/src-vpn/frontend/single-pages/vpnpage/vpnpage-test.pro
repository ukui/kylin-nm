QT += core gui widgets dbus testlib network svg x11extras

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_vpnpage

INCLUDEPATH += ../../../../src-vpn/frontend
INCLUDEPATH += ../../../../src-vpn/frontend/single-pages
INCLUDEPATH += ../../../../src-vpn/frontend/list-items
INCLUDEPATH += ../../../../src/frontend/tools
INCLUDEPATH += ../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/KF5/KWindowSystem

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
LIBS += -lgcov
LIBS += $$PKGCONFIG_LIBS
LIBS += -lKF5WindowSystem

SOURCES += ../../../../src-vpn/frontend/single-pages/singlepage.cpp
SOURCES += ../../../../src-vpn/frontend/single-pages/vpnpage.cpp
SOURCES += ../../../../src/frontend/tools/divider.cpp
SOURCES += ../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../src-vpn/frontend/list-items/listitem.cpp
SOURCES += ../../../../src-vpn/frontend/list-items/vpnlistitem.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectresource.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp
SOURCES += ../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kyvpnconnectoperation.cpp
SOURCES += ../../../../common/common.cpp
SOURCES += $$PWD/test_vpnpage.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../src-vpn/frontend/single-pages/singlepage.h
HEADERS += ../../../../src-vpn/frontend/single-pages/vpnpage.h
HEADERS += ../../../../src/frontend/tools/divider.h
HEADERS += ../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../src-vpn/frontend/list-items/listitem.h
HEADERS += ../../../../src-vpn/frontend/list-items/vpnlistitem.h
HEADERS += ../../../../common/common.h
HEADERS += $$PWD/test_vpnpage.h
