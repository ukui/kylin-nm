QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt no_keywords
CONFIG += link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kylinagent

INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../common
INCLUDEPATH += ../../../../../src/backend/dbus-interface

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt

SOURCES += ../../../../../src/backend/dbus-interface/kylinagent.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinsecretagent.cpp
SOURCES += test_kylinagent.cpp
SOURCES += main.cpp

HEADERS += ../../../../../src/backend/dbus-interface/kylinagent.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinsecretagent.h
HEADERS += test_kylinagent.h
