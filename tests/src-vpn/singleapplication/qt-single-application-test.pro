QT       += core gui widgets network dbus x11extras

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

TARGET = test_qt-single-application

INCLUDEPATH += ./stub_headers
INCLUDEPATH += ../../../../src-vpn/singleapplication
INCLUDEPATH += ../../../../src-vpn/frontend
INCLUDEPATH += ../../../../src/frontend/tools
INCLUDEPATH += ../../../../src-vpn/frontend/single-pages
INCLUDEPATH += ../../../../src-vpn/frontend/single-pages/list-items
INCLUDEPATH += ../../../../src-vpn/frontend/vpndetails
INCLUDEPATH += /usr/include/gtest
INCLUDEPATH += /usr/include/gmock
INCLUDEPATH += /usr/include/KF5
INCLUDEPATH += /usr/include/KF5/KWindowSystem
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QGSettings
INCLUDEPATH += ../../../../src/backend/dbus-interface

PKGCONFIG += gio-2.0 gio-unix-2.0 glib-2.0 gsettings-qt KWindowSystem

LIBS += -lgtest -lgtest_main -lgmock -lpthread

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov --coverage

DEFINES += QT_NO_DEBUG

SOURCES += \
    ../../../../src-vpn/singleapplication/qt-single-application.cpp \
    ../../../../src-vpn/singleapplication/qt-local-peer.cpp \
    ../../../../src-vpn/singleapplication/qt-locked-file.cpp \
    ../../../../src-vpn/singleapplication/qt-locked-file-unix.cpp \
    test_qt-single-application.cpp \
    main.cpp

HEADERS += \
    ../../../../src-vpn/singleapplication/qt-single-application.h \
    ../../../../src-vpn/singleapplication/qt-local-peer.h \
    ../../../../src-vpn/singleapplication/qt-locked-file.h \
    test_qt-single-application.h \
    stub_headers/vpnobject.h \
    stub_headers/kylinactiveconnectresource.h \
    stub_headers/ukuiwindowhelper.h
