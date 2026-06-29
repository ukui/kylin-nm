QT += core gui widgets dbus testlib x11extras network KWindowSystem

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_netdetail

INCLUDEPATH += $$PWD/../../../../../src/frontend
INCLUDEPATH += $$PWD/../../../../../src/frontend/netdetails
INCLUDEPATH += $$PWD/../../../../../src/frontend/tools
INCLUDEPATH += $$PWD/../../../../../src/frontend/networkmode
INCLUDEPATH += $$PWD/../../../../../src/frontend/tab-pages
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

SOURCES += ../../../../../src/frontend/netdetails/netdetail.cpp
SOURCES += ../../../../../src/frontend/netdetails/detailpage.cpp
SOURCES += ../../../../../src/frontend/netdetails/detailwidget.cpp
SOURCES += ../../../../../src/frontend/netdetails/ipv4page.cpp
SOURCES += ../../../../../src/frontend/netdetails/ipv6page.cpp
SOURCES += ../../../../../src/frontend/netdetails/securitypage.cpp
SOURCES += ../../../../../src/frontend/netdetails/creatnetpage.cpp
SOURCES += ../../../../../src/frontend/netdetails/configpage.cpp
SOURCES += ../../../../../src/frontend/netdetails/multiplednswidget.cpp
SOURCES += ../../../../../src/frontend/netdetails/customtabstyle.cpp
SOURCES += ../../../../../src/frontend/netdetails/joinhiddenwifipage.cpp
SOURCES += ../../../../../src/frontend/netdetails/dnssettingwidget.cpp
SOURCES += ../../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../../src/frontend/tools/themepalette.cpp
SOURCES += test_netdetail.cpp

HEADERS += test_netdetail.h
HEADERS += ../../../../../src/frontend/netdetails/netdetail.h
