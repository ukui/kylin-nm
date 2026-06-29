QT += core gui widgets dbus testlib network svg x11extras

CONFIG += console c++14 qt no_keyword
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_vpnconfigpage

INCLUDEPATH += ../../../../../src-vpn/frontend
INCLUDEPATH += ../../../../../src-vpn/frontend/vpndetails
INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../common
INCLUDEPATH += ../../../common
INCLUDEPATH += ../../../../../src/frontend/tools
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
LIBS += -lKF5NetworkManagerQt

SOURCES += ../../../../../src-vpn/frontend/vpndetails/vpnconfigpage.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kyvpnconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinapconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp
SOURCES += ../../../../../src/frontend/tools/kylable.cpp
SOURCES += $$PWD/test_vpnconfigpage.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../../src-vpn/frontend/vpndetails/vpnconfigpage.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kyvpnconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinapconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h
HEADERS += ../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../common/common.h
HEADERS += $$PWD/test_vpnconfigpage.h
