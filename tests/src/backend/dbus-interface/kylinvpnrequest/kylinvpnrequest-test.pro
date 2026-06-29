QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt no_keywords

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kylinvpnrequest

INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../common
INCLUDEPATH += ../../../../../src/backend/dbus-interface

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap gtk+-3.0

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt
LIBS += -lnma

# Compile the .c file as C++ by adding it to SOURCES but with special handling
QMAKE_EXTRA_COMPILERS += cplusplus_source
cplusplus_source.input = $$PWD/../../../../../src/backend/dbus-interface/kylinvpnrequest.c
cplusplus_source.output = ${OBJDIR}/kylinvpnrequest.o
cplusplus_source.commands = $$QMAKE_CXX -x c++ -c $$QMAKE_CXXFLAGS $$(PKG_CFLAGS) -I../../../../../src -I../../../../../common -I../../../../../src/backend/dbus-interface -I/usr/include/KF5/NetworkManagerQt -o ${OBJDIR}/kylinvpnrequest.o $$<
cplusplus_source.dependency_type = TYPE_C

SOURCES += test_kylinvpnrequest.cpp
SOURCES += main.cpp

HEADERS += ../../../../../src/backend/dbus-interface/kylinvpnrequest.h
HEADERS += test_kylinvpnrequest.h
