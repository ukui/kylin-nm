QT       += core gui x11extras dbus KWindowSystem svg concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console c++14 qt

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_main

INCLUDEPATH += ../../../src-vpn
INCLUDEPATH += ../../../src-vpn/singleapplication
INCLUDEPATH += ../../../src-vpn/frontend
INCLUDEPATH += ../../../src-vpn/frontend/single-pages
INCLUDEPATH += ../../../src-vpn/frontend/list-items
INCLUDEPATH += ../../../src-vpn/frontend/vpndetails
INCLUDEPATH += ../../../src-vpn/backend
INCLUDEPATH += ../../../common
INCLUDEPATH += ../../../pub
INCLUDEPATH += ../../../src/frontend/tools
INCLUDEPATH += ../../../src/backend/dbus-interface
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/include/libnm
INCLUDEPATH += /usr/include/kysdk/applications
INCLUDEPATH += /usr/include/kysdk/kysdk-system
INCLUDEPATH += /usr/include/kysdk/sysinfo
INCLUDEPATH += /usr/include/kysdk/ukui
INCLUDEPATH += /usr/include/libsecret-1
INCLUDEPATH += /usr/include/KF5/KWindowSystem
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/dbus-1.0

# 自动跨架构核心
INCLUDEPATH += $$[QT_INSTALL_HEADERS]
INCLUDEPATH += $$[QT_INSTALL_LIBS]/glib-2.0/include
INCLUDEPATH += $$[QT_INSTALL_LIBS]/dbus-1.0/include


PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap dbus-1

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

# Add required libraries for linking
LIBS += -lKF5NetworkManagerQt -lukui-log4qt -lgsettings-qt -lX11 -lgio-2.0 -lgobject-2.0 -lglib-2.0

LIBS += -lgtest -lgtest_main -lpthread

HEADERS +=../../../src-vpn/singleapplication/qt-single-application.h \
../../../src-vpn/singleapplication/qt-local-peer.h \
../../../src-vpn/singleapplication/qt-locked-file.h \
../../../src-vpn/frontend/vpnobject.h  \
../../../src-vpn/backend/vpndbusadaptor.h \
../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h  \
../../../src/backend/dbus-interface/kylinconnectresource.h  \
../../../src/backend/dbus-interface/kylinconnectsetting.h \
../../../src/backend/dbus-interface/kylinactiveconnectresource.h \
../../../src/backend/dbus-interface/kylinconnectitem.h  \
../../../src/backend/dbus-interface/kylinconnectoperation.h \
../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h   \
../../../src/backend/dbus-interface/kylinutil.h \
../../../src/backend/dbus-interface/kylinvpnconnectitem.h \
../../../src/backend/dbus-interface/kyenterpricesettinginfo.h

SOURCES += ../../../src-vpn/main.cpp  \
../../../src-vpn/singleapplication/qt-single-application.cpp \
../../../src-vpn/singleapplication/qt-local-peer.cpp \
../../../src-vpn/singleapplication/qt-locked-file.cpp \
../../../src-vpn/singleapplication/qt-locked-file-unix.cpp  \
../../../src-vpn/frontend/vpnobject.cpp  \
../../../src-vpn/backend/vpndbusadaptor.cpp \
../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp  \
../../../src/backend/dbus-interface/kylinconnectresource.cpp  \
../../../src/backend/dbus-interface/kylinconnectsetting.cpp \
../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp \
../../../src/backend/dbus-interface/kylinconnectitem.cpp  \
../../../src/backend/dbus-interface/kylinconnectoperation.cpp \
../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp   \
../../../src/backend/dbus-interface/kylinutil.cpp \
../../../src/backend/dbus-interface/kylinvpnconnectitem.cpp \
../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp \
test_main.cpp  \
test_main_entry.cpp

# Rename main function in main.cpp to avoid conflict
QMAKE_CXXFLAGS += -Dmain=original_main
