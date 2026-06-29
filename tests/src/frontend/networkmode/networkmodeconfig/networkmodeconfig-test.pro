QT += core gui widgets dbus testlib network svg x11extras

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_networkmodeconfig

INCLUDEPATH += ../../../../../../src/frontend
INCLUDEPATH += ../../../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../../src/frontend/enterprise-wlan
INCLUDEPATH += ../../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../../src/frontend/connectivity
INCLUDEPATH += ../../../../../../src/frontend/xatom
INCLUDEPATH += ../../../../../../src/frontend/list-items
INCLUDEPATH += ../../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../../src
INCLUDEPATH += ../../../../../../src/backend
INCLUDEPATH += ../../../../../../pub
INCLUDEPATH += ../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/KF5/KWindowSystem
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QGSettings
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/libnm
INCLUDEPATH += /usr/include/kysdk
INCLUDEPATH += /usr/include/kysdk/applications

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo kysdk-ukuiwindowhelper

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
LIBS += -lgcov
LIBS += $$PKGCONFIG_LIBS
LIBS += -lKF5WindowSystem
LIBS += -lKF5NetworkManagerQt
LIBS += -lgtest -lgtest_main -lpthread

SOURCES += ../../../../../../src/frontend/networkmode/networkmodeconfig.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectresource.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../../../src/backend/utils.cpp
SOURCES += $$PWD/test_networkmodeconfig.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../../../src/frontend/networkmode/networkmodeconfig.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinconnectresource.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../../src/backend/utils.h
HEADERS += ../../../common/common.h
HEADERS += $$PWD/test_networkmodeconfig.h

