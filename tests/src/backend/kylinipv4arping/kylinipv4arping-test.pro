QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kylinipv4arping

INCLUDEPATH += ../../../../src/backend
INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../common

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread

SOURCES += \
    ../../../../src/backend/kylinipv4arping.cpp \
    test_kylinipv4arping.cpp \
    main.cpp

HEADERS += \
    ../../../../src/backend/kylinipv4arping.h \
    ../../../../src/backend/kylinarping.h \
    test_kylinipv4arping.h
