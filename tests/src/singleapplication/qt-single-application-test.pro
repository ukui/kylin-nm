QT += core widgets network dbus testlib

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_qt-single-application

INCLUDEPATH += ../../../src
INCLUDEPATH += ../../../src/singleapplication
INCLUDEPATH += ../../../src/frontend
INCLUDEPATH += ../../../src/frontend/connectivity
INCLUDEPATH += ../../../src/frontend/enterprise-wlan
INCLUDEPATH += ../../../src/frontend/list-items
INCLUDEPATH += ../../../src/frontend/netdetails
INCLUDEPATH += ../../../src/frontend/networkmode
INCLUDEPATH += ../../../src/frontend/tab-pages
INCLUDEPATH += ../../../src/frontend/tools
INCLUDEPATH += ../../../src/frontend/xatom
INCLUDEPATH += ../../../src/backend
INCLUDEPATH += ../../../src/backend/dbus-interface
INCLUDEPATH += ../../../common
INCLUDEPATH += ../../../pub
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QGSettings
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/libnm
INCLUDEPATH += /usr/include/kysdk/applications
INCLUDEPATH += /usr/include/kysdk/kysdk-system
INCLUDEPATH += /usr/include/kysdk/sysinfo
INCLUDEPATH += /usr/include/kysdk/ukui
INCLUDEPATH += /usr/include/libsecret-1
INCLUDEPATH += /usr/include/KF5/KWindowSystem
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += ../../../src/singleapplication/qt-single-application.cpp ../../../src/singleapplication/qt-local-peer.cpp ../../../src/singleapplication/qt-locked-file.cpp ../../../src/singleapplication/qt-locked-file-unix.cpp test_qt-single-application.cpp main.cpp

HEADERS += test_qt-single-application.h
