QT += core gui widgets dbus testlib x11extras network

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_connectivitypage

INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../src/frontend/connectivity
INCLUDEPATH += ../../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../src/frontend/list-items
INCLUDEPATH += ../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common

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
QMAKE_LIBS += -lKF5WindowSystem
QMAKE_LIBS += -lKF5NetworkManagerQt

SOURCES += ../../../../../src/frontend/connectivity/connectivitypage.cpp
SOURCES += ../../../../../src/frontend/tools/divider.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinapconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kywirelessnetitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kywirelessnetresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp
SOURCES += test_connectivitypage.cpp
SOURCES += main.cpp

HEADERS += ../../../../../src/frontend/connectivity/connectivitypage.h
HEADERS += ../../../../../src/frontend/tools/divider.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinapconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kywirelessnetitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kywirelessnetresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h
HEADERS += ../../../../../src/backend/dbus-interface/gsystem-local-alloc.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += test_connectivitypage.h
