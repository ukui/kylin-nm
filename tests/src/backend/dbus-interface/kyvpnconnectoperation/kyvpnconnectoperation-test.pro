QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kyvpnconnectoperation

INCLUDEPATH += $$PWD/../../../../../../src
INCLUDEPATH += $$PWD/../../../../../../common
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_KEYWORDS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt

SOURCES += $$PWD/test_kyvpnconnectoperation.cpp \
           $$PWD/main.cpp \
           ../../../../../../src/backend/dbus-interface/kyvpnconnectoperation.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp \
           ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp \
           ../../../../../../src/backend/dbus-interface/kylinutil.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectitem.cpp \
           ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectresource.cpp \
           ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp \
           ../../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp \
           ../../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp \
           ../../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp \
           ../../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.cpp \
           ../../../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp \
           ../../../../../../src/backend/dbus-interface/kylinapconnectitem.cpp

HEADERS += $$PWD/test_kyvpnconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kyvpnconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectsetting.h \
           ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h \
           ../../../../../../src/backend/dbus-interface/kylinutil.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectitem.h \
           ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectresource.h \
           ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h \
           ../../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h \
           ../../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.h \
           ../../../../../../src/backend/dbus-interface/kylinvpnconnectitem.h \
           ../../../../../../src/backend/dbus-interface/kylinapconnectitem.h
