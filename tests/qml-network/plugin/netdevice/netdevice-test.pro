QT += core dbus testlib gui

CONFIG += qt console warn_on depend_incremental
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_netdevice

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../../../../qml-network/plugin
INCLUDEPATH += ../../../../qml-network
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../../pub


LIBS += -lpthread

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage -Wl,--no-as-needed

SOURCES += main.cpp
SOURCES += test_netdevice.cpp
SOURCES += ../../../../qml-network/plugin/netdevice.cpp

HEADERS += test_netdevice.h
HEADERS += ../../../../qml-network/plugin/netdevice.h
