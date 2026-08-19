# Kylin-NM Unit Test for vpnsecrets

QT += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_vpnsecrets
TEMPLATE = app

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords

PKGCONFIG += libnm libsecret-1 gtk+-3.0 gio-2.0 glib-2.0 gio-unix-2.0 dbus-1 dbus-glib-1

INCLUDEPATH += ../../../../src/backend/vpnsecrets
INCLUDEPATH += ../../../../src/backend/dbus-interface
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lgcov


DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_LFLAGS += -lgcov -fprofile-arcs -ftest-coverage
QMAKE_CFLAGS += --coverage -fno-inline
QMAKE_CXXFLAGS += --coverage -fno-inline -fno-access-control -fno-exceptions

SOURCES += main.cpp \
           test_vpnsecrets.cpp \
           ../../../../src/backend/vpnsecrets/kylinvpnrequest.c \
           ../../../../src/backend/vpnsecrets/kylinvpnagent.c \
           ../../../../src/backend/vpnsecrets/nma-vpn-password-dialog.c \

HEADERS += test_vpnsecrets.h \
           ../../../../src/backend/vpnsecrets/kylinvpnrequest.h \
           ../../../../src/backend/vpnsecrets/kylinvpnagent.h \
           ../../../../src/backend/vpnsecrets/nma-vpn-password-dialog.c \
           ../../../../src/backend/dbus-interface/nm-macros-internal.h