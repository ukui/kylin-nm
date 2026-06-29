QT += core gui testlib dbus x11extras network widgets

CONFIG += qt console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test_vpndbusadaptor

INCLUDEPATH += $$PWD/../../../../src-vpn
INCLUDEPATH += $$PWD/../../../../src-vpn/singleapplication
INCLUDEPATH += $$PWD/../../../../src-vpn/frontend
INCLUDEPATH += $$PWD/../../../../src-vpn/frontend/single-pages
INCLUDEPATH += $$PWD/../../../../src-vpn/frontend/list-items
INCLUDEPATH += $$PWD/../../../../src-vpn/frontend/vpndetails
INCLUDEPATH += $$PWD/../../../../src-vpn/backend
INCLUDEPATH += $$PWD/../../../../common
INCLUDEPATH += $$PWD/../../../../pub
INCLUDEPATH += $$PWD/../../../../src/frontend/tools
INCLUDEPATH += $$PWD/../../../../src/backend/dbus-interface
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
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap dbus-1

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt -lukui-log4qt -lgsettings-qt -lX11 -lgio-2.0 -lgobject-2.0 -lglib-2.0

SOURCES += test_vpndbusadaptor.cpp main.cpp ../../../../src-vpn/backend/vpndbusadaptor.cpp ../../../../src-vpn/frontend/vpnobject.cpp ../../../../src-vpn/singleapplication/qt-single-application.cpp ../../../../src-vpn/singleapplication/qt-local-peer.cpp ../../../../src-vpn/singleapplication/qt-locked-file.cpp ../../../../src-vpn/singleapplication/qt-locked-file-unix.cpp ../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp ../../../../src/backend/dbus-interface/kylinconnectresource.cpp ../../../../src/backend/dbus-interface/kylinconnectsetting.cpp ../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp ../../../../src/backend/dbus-interface/kylinconnectitem.cpp ../../../../src/backend/dbus-interface/kylinconnectoperation.cpp ../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp ../../../../src/backend/dbus-interface/kylinutil.cpp ../../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp ../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp

HEADERS += test_vpndbusadaptor.h
