QT += core gui widgets dbus testlib network

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_securitypage

INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

LIBS += -lKF5NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

SOURCES += ../../../../../src/frontend/netdetails/securitypage.cpp
SOURCES += ../../../../../src/frontend/tools/listitemedit.cpp
SOURCES += ../../../../../src/frontend/tools/divider.cpp
SOURCES += ../../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../../src/frontend/netdetails/multiplednswidget.cpp
SOURCES += ../../../../../src/frontend/netdetails/dnssettingwidget.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += test_securitypage.cpp
SOURCES += main.cpp

HEADERS += test_securitypage.h
HEADERS += ../../../../../src/frontend/netdetails/securitypage.h
HEADERS += ../../../../../src/frontend/tools/listitemedit.h
HEADERS += ../../../../../src/frontend/tools/divider.h
HEADERS += ../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../src/frontend/netdetails/multiplednswidget.h
HEADERS += ../../../../../src/frontend/netdetails/dnssettingwidget.h
HEADERS += ../../../../../src/frontend/netdetails/coninfo.h
