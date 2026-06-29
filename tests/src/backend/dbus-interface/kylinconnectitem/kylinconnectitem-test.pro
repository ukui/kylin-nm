QT += core gui testlib dbus x11extras network widgets

CONFIG += qt console
CONFIG -= app_bundle
CONFIG += link_pkgconfig

TEMPLATE = app
TARGET = test_kylinconnectitem

INCLUDEPATH += ../../../../../common
INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/dbus-1.0

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap dbus-1

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_KEYWORDS

QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt

SOURCES = test_kylinconnectitem.cpp ../../../../../src/backend/dbus-interface/kylinconnectitem.cpp ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp ../../../../../src/backend/dbus-interface/kylinutil.cpp

HEADERS = test_kylinconnectitem.h ../../../../../src/backend/dbus-interface/kylinconnectitem.h ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h ../../../../../src/backend/dbus-interface/kylinutil.h
