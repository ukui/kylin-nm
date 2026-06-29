QT += core gui testlib dbus x11extras network widgets

CONFIG += qt console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test_kywirelessnetitem

INCLUDEPATH += $$PWD/../../../../../common
INCLUDEPATH += $$PWD/../../../../../src/backend/dbus-interface
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap dbus-1

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

SOURCES = ../test_kywirelessnetitem.cpp $$PWD/main.cpp ../../../../../src/backend/dbus-interface/kywirelessnetitem.cpp ../../../../../src/backend/dbus-interface/kylinutil.cpp ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp

HEADERS = ../test_kywirelessnetitem.h ../../../../../src/backend/dbus-interface/kywirelessnetitem.h ../../../../../src/backend/dbus-interface/kylinutil.h ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
