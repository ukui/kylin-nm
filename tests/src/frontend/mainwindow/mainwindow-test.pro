QT += core gui x11extras dbus KWindowSystem svg concurrent network testlib

CONFIG += console c++14 qt link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_mainwindow

INCLUDEPATH += ../../../../src/frontend
INCLUDEPATH += ../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../src/frontend/tools
INCLUDEPATH += ../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../src/frontend/enterprise-wlan
INCLUDEPATH += ../../../../src/frontend/connectivity
INCLUDEPATH += ../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../src/frontend/xatom
INCLUDEPATH += ../../../../src/frontend/list-items
INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../src/backend
INCLUDEPATH += ../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

LIBS += -L/usr/lib/ -lKF5NetworkManagerQt


DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

SOURCES += ../../../../src/frontend/mainwindow.cpp
SOURCES += test_mainwindow.cpp
SOURCES += main.cpp
SOURCES += ../../../../src/frontend/connectivity/connectivitypage.cpp
SOURCES += ../../../../src/frontend/tab-pages/tabpage.cpp
SOURCES += ../../../../src/frontend/tab-pages/lanpage.cpp
SOURCES += ../../../../src/frontend/tab-pages/wlanpage.cpp
SOURCES += ../../../../src/frontend/netdetails/joinhiddenwifipage.cpp
SOURCES += ../../../../src/frontend/customstyle.cpp

SOURCES += ../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinsecretagent.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp

HEADERS += test_mainwindow.h
HEADERS += ../../../../src/frontend/mainwindow.h
HEADERS += ../../../../src/frontend/connectivity/connectivitypage.h
HEADERS += ../../../../src/frontend/tab-pages/tabpage.h
HEADERS += ../../../../src/frontend/tab-pages/lanpage.h
HEADERS += ../../../../src/frontend/tab-pages/wlanpage.h
HEADERS += ../../../../src/frontend/netdetails/joinhiddenwifipage.h
HEADERS += ../../../../src/frontend/customstyle.h

HEADERS += ../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += ../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h
HEADERS += ../../../../src/backend/dbus-interface/kylinsecretagent.h
HEADERS += ../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
