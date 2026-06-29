QT += core gui widgets testlib x11extras dbus KWindowSystem concurrent network

CONFIG += qt console
CONFIG -= app_bundle

CONFIG += c++14
CONFIG += debug

INCLUDEPATH += $$PWD/../../../../src/frontend
INCLUDEPATH += $$PWD/../../../../src/backend
INCLUDEPATH += $$PWD/../../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

LIBS += -L/usr/lib/ -lgsettings-qt -lX11 -lKF5NetworkManagerQt -lukui-log4qt -lkysdk-ukuiwindowhelper

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += test_wificonfigdialog.cpp ../../../../src/backend/utils.cpp ../../../../src/frontend/wificonfigdialog.cpp

HEADERS += test_wificonfigdialog.h ../../../../src/frontend/wificonfigdialog.h ../../../../src/backend/utils.h ../../../../common/common.h

FORMS += ../../../../src/frontend/wificonfigdialog.ui

TARGET = test_wificonfigdialog

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
