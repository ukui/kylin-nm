QT += core gui widgets dbus testlib network

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_divider

INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/tab-pages
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/tools
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/netdetails
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/enterprise-wlan
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/connectivity
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/networkmode
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/xatom
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/list-items
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/backend/dbus-interface
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/common
INCLUDEPATH += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/tests/common

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += -fPIC
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

SOURCES += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/tools/divider.cpp
SOURCES += $$PWD/test_divider.cpp
SOURCES += $$PWD/main.cpp

HEADERS += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/tools/divider.h
HEADERS += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/src/frontend/tools/themepalette.h
HEADERS += $$PWD/test_divider.h
