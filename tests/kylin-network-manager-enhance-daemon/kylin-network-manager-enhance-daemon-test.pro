# Kylin-NM Unit Test for kylin-network-manager-enhance-daemon

QT += core gui dbus x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_kylin_network_manager_enhance_daemon
TEMPLATE = app

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 dbus-1 dbus-glib-1

INCLUDEPATH += ../../kylin-network-manager-enhance-daemon
INCLUDEPATH += ../../kylin-network-manager-enhance-daemon/kylin/common
INCLUDEPATH += ../../kylin-network-manager-enhance-daemon/kylin/netControl
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lgcov
LIBS += -ldbus-1
LIBS += -lglib-2.0

DEFINES += QT_DEPRECATED_WARNINGS


QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_CFLAGS += --coverage -fno-inline
QMAKE_CXXFLAGS += --coverage -fno-inline -fno-access-control -fno-exceptions

SOURCES += main.cpp \
           test_daemon.cpp \
           ../../kylin-network-manager-enhance-daemon/file_monitor.c \
           ../../kylin-network-manager-enhance-daemon/dbus-server.c \


HEADERS += test_daemon.h \
           ../../kylin-network-manager-enhance-daemon/public.h \
           ../../kylin-network-manager-enhance-daemon/file_monitor.h \
           ../../kylin-network-manager-enhance-daemon/dbus-server.h \
           ../../kylin-network-manager-enhance-daemon/dns-xml.h