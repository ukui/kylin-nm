QT += core gui widgets dbus network testlib x11extras  KWindowSystem svg concurrent

CONFIG += console c++14 qt no_keywords

CONFIG -= app_bundle
CONFIG += link_pkgconfig

TEMPLATE = app

TARGET = test_kylinactiveconnectresource

INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../../src/backend
INCLUDEPATH += ../../../../pub

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LIBS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt

SOURCES += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinapconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../pub/appsettings.cpp
SOURCES += ../../../../../pub/uisecurityconfig.cpp
SOURCES += test_kylinactiveconnectresource.cpp
SOURCES += main.cpp

HEADERS += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinvpnconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinapconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../../pub/appsettings.h
HEADERS += ../../../../../pub/uisecurityconfig.h
HEADERS += test_kylinactiveconnectresource.h
