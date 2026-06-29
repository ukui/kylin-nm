QT += core gui widgets dbus testlib x11extras network

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_enterprisewlandialog

INCLUDEPATH += $$PWD/../../../../src/frontend
INCLUDEPATH += $$PWD/../../../../src/frontend/enterprise-wlan
INCLUDEPATH += $$PWD/../../../../src/frontend/tab-pages
INCLUDEPATH += $$PWD/../../../../src/frontend/tools
INCLUDEPATH += $$PWD/../../../../src/frontend/netdetails
INCLUDEPATH += $$PWD/../../../../src/frontend/list-items
INCLUDEPATH += $$PWD/../../../../src/frontend/networkmode
INCLUDEPATH += $$PWD/../../../../src
INCLUDEPATH += $$PWD/../../../../src/backend/dbus-interface
INCLUDEPATH += $$PWD/../../../../common
INCLUDEPATH += $$PWD/../../../common

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

SOURCES += ../../../../src/frontend/enterprise-wlan/enterprisewlandialog.cpp
SOURCES += ../../../../src/frontend/tools/divider.cpp
SOURCES += ../../../../src/frontend/netdetails/securitypage.cpp
SOURCES += ../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../src/backend/dbus-interface/kywirelessnetitem.cpp
SOURCES += ../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kywirelessnetresource.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectresource.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinapconnectitem.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += ../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += $$PWD/test_enterprisewlandialog.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../src/frontend/enterprise-wlan/enterprisewlandialog.h
HEADERS += ../../../../src/frontend/tools/divider.h
HEADERS += ../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../src/frontend/netdetails/securitypage.h
HEADERS += ../../../../src/frontend/netdetails/coninfo.h
HEADERS += ../../../../src/backend/dbus-interface/kywirelessnetitem.h
HEADERS += ../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../src/backend/dbus-interface/kywirelessnetresource.h
HEADERS += ../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../src/backend/dbus-interface/kylinconnectresource.h
HEADERS += ../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += ../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h
HEADERS += ../../../../src/backend/dbus-interface/kylinapconnectitem.h
HEADERS += ../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.h
HEADERS += ../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += ../../../../src/backend/dbus-interface/kylinvpnconnectitem.h
HEADERS += $$PWD/test_enterprisewlandialog.h