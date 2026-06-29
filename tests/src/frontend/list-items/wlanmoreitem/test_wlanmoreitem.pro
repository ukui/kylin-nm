QT       += core gui x11extras dbus KWindowSystem svg concurrent network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_wlanmoreitem
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
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QGSettings
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/libnm
INCLUDEPATH += /usr/include/kysdk
INCLUDEPATH += /usr/include/kysdk/applications

SOURCES += test_wlanmoreitem.cpp \
    ../../../../../src/frontend/list-items/wlanmoreitem.cpp \
    ../../../../../src/frontend/list-items/listitem.cpp \
    ../../../../../src/frontend/tools/radioitembutton.cpp \
    ../../../../../src/frontend/tools/infobutton.cpp \
    ../../../../../src/frontend/tools/kylable.cpp \
    ../../../../../src/frontend/netdetails/netdetail.cpp \
    ../../../../../src/frontend/netdetails/detailpage.cpp \
    ../../../../../src/frontend/netdetails/detailwidget.cpp \
    ../../../../../src/frontend/networkmode/networkmodeconfig.cpp \
    ../../../../../src/backend/dbus-interface/kylinconnectitem.cpp \
    ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp \
    ../../../../../src/backend/dbus-interface/kywirelessnetitem.cpp

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

LIBS    += -L/usr/lib/ -lgsettings-qt -lX11 -lKF5NetworkManagerQt -lukui-log4qt -lkysdk-ukuiwindowhelper

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_CFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
