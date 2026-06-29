QT       += core gui x11extras dbus KWindowSystem svg concurrent network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_vpnlistitem
TEMPLATE = app

CONFIG += c++14 qt warn_on

INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../src/frontend/list-items
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../src/backend
INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common
INCLUDEPATH += ../../../../../src-vpn/frontend
INCLUDEPATH += ../../../../../src-vpn/frontend/list-items
INCLUDEPATH += ../../../../../src-vpn/frontend/vpndetails
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QGSettings
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/libnm
INCLUDEPATH += /usr/include/kysdk
INCLUDEPATH += /usr/include/kysdk/applications

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

LIBS    += -L/usr/lib/ -lgsettings-qt -lX11 -lKF5NetworkManagerQt -lukui-log4qt -lkysdk-ukuiwindowhelper

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

SOURCES += ../../../../../src-vpn/frontend/list-items/vpnlistitem.cpp
SOURCES += ../../../../../src-vpn/frontend/list-items/listitem.cpp
SOURCES += ../../../../../src/frontend/tools/radioitembutton.cpp
SOURCES += ../../../../../src/frontend/tools/infobutton.cpp
SOURCES += ../../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../../src/frontend/netdetails/netdetail.cpp
SOURCES += ../../../../../src/frontend/networkmode/networkmodeconfig.cpp
SOURCES += ../../../../../src-vpn/frontend/vpndetails/vpndetail.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectitem.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kyvpnconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinsecretagent.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../src/frontend/customstyle.cpp
SOURCES += $$PWD/test_vpnlistitem.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../../src-vpn/frontend/list-items/vpnlistitem.h

HEADERS += ../../../../../src-vpn/frontend/list-items/vpnlistitem.h
HEADERS += ../../../../../src-vpn/frontend/list-items/listitem.h
HEADERS += ../../../../../src/frontend/tools/radioitembutton.h
HEADERS += ../../../../../src/frontend/tools/infobutton.h
HEADERS += ../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../src/frontend/netdetails/netdetail.h
HEADERS += ../../../../../src-vpn/frontend/vpndetails/vpndetail.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectitem.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../src/backend/dbus-interface/kyvpnconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
HEADERS += $$PWD/test_vpnlistitem.h