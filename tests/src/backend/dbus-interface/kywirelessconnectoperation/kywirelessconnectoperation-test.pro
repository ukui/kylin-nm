QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kywirelessconnectoperation

INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap dbus-1

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig no_keywords

QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt
LIBS += -ldbus-1

SOURCES += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp
SOURCES += test_kywirelessconnectoperation.cpp
SOURCES += main.cpp

HEADERS += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h
HEADERS += test_kywirelessconnectoperation.h
