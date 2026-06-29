QT += core gui widgets dbus testlib x11extras network KWindowSystem

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_detailpage

INCLUDEPATH += $$PWD/../../../../../src/frontend
INCLUDEPATH += $$PWD/../../../../../src/frontend/netdetails
INCLUDEPATH += $$PWD/../../../../../src/frontend/tools
INCLUDEPATH += $$PWD/../../../../../src
INCLUDEPATH += $$PWD/../../../../../common
INCLUDEPATH += $$PWD/../../../../../src/backend/dbus-interface
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS
QMAKE_LFLAGS += -lKF5NetworkManagerQt
LIBS += -lKF5NetworkManagerQt

SOURCES += ../../../../../src/frontend/netdetails/detailpage.cpp
SOURCES += ../../../../../src/frontend/netdetails/detailwidget.cpp
SOURCES += ../../../../../src/frontend/tools/kylable.cpp
SOURCES += test_detailpage.cpp
SOURCES += main.cpp

HEADERS += test_detailpage.h
HEADERS += ../../../../../src/frontend/netdetails/detailpage.h
HEADERS += ../../../../../src/frontend/netdetails/detailwidget.h
HEADERS += ../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../src/frontend/netdetails/coninfo.h