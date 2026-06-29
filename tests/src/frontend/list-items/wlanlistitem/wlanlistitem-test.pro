QT += core gui widgets dbus testlib network svg x11extras

CONFIG += console c++14 qt no_keywords
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_wlanlistitem

INCLUDEPATH += ../../../../../../src/frontend
INCLUDEPATH += ../../../../../../src/frontend/list-items
INCLUDEPATH += ../../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../../src/frontend/enterprise-wlan
INCLUDEPATH += ../../../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../../src/frontend/connectivity
INCLUDEPATH += ../../../../../../src/frontend/xatom
INCLUDEPATH += ../../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../../src
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../common
INCLUDEPATH += /usr/include/KF5/KWindowSystem

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

LIBS += -lKF5WindowSystem
LIBS += -lKF5NetworkManagerQt
LIBS += -lgtest -lgtest_main -lpthread

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

SOURCES += ../../../../../../src/frontend/list-items/wlanlistitem.cpp
SOURCES += ../../../../../../src/frontend/list-items/listitem.cpp
SOURCES += ../../../../../../src/frontend/tools/radioitembutton.cpp
SOURCES += ../../../../../../src/frontend/tools/infobutton.cpp
SOURCES += ../../../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../../../src/frontend/netdetails/netdetail.cpp
SOURCES += ../../../../../../src/frontend/customstyle.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kywirelessnetitem.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinsecretagent.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += $$PWD/test_wlanlistitem.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../../../src/frontend/list-items/wlanlistitem.h
HEADERS += ../../../../../../src/frontend/list-items/listitem.h
HEADERS += ../../../../../../src/frontend/tools/radioitembutton.h
HEADERS += ../../../../../../src/frontend/tools/infobutton.h
HEADERS += ../../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../../src/frontend/netdetails/netdetail.h
HEADERS += ../../../../../../src/frontend/customstyle.h
HEADERS += ../../../../../../src/backend/dbus-interface/kywirelessnetitem.h
HEADERS += ../../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinsecretagent.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += $$PWD/test_wlanlistitem.h
