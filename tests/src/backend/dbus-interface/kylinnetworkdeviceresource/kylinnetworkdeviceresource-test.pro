QT += core gui dbus network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_kylinnetworkdeviceresource
TEMPLATE = app

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 dbus-1 libnm gsettings-qt

INCLUDEPATH += $$PWD/../../../../../src/backend/dbus-interface
INCLUDEPATH += $$PWD/../../../../../common
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lgcov
LIBS += -ldbus-1
LIBS += -lNetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += $$PWD/main.cpp 
SOURCES += $$PWD/test_kylinnetworkdeviceresource.cpp 
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kylinapconnectitem.cpp
SOURCES += $$PWD/../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp

HEADERS += $$PWD/test_kylinnetworkdeviceresource.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinbluetoothconnectitem.h
HEADERS += $$PWD/../../../../../src/backend/dbus-interface/kylinapconnectitem.h
HEADERS += $$PWD/../../../../../common/kylinnetworkresourcemanager.h
