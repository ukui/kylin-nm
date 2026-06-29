QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_dlghotspotcreate

UI_DIR = $$PWD/.ui
MOC_DIR = $$PWD/.moc

INCLUDEPATH += ../../../../../src/backend
INCLUDEPATH += ../../../../../src/backend/hotspot
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../src/frontend/tab-pages
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../common

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

FORMS += ../../../../../src/backend/hotspot/dlghotspotcreate.ui

SOURCES += ../../../../../src/backend/utils.cpp
SOURCES += ../../../../../src/backend/hotspot/dlghotspotcreate.cpp
SOURCES += $$PWD/test_dlghotspotcreate.cpp
SOURCES += $$PWD/main.cpp

HEADERS += ../../../../../src/backend/utils.h
HEADERS += ../../../../../src/backend/hotspot/dlghotspotcreate.h
HEADERS += $$PWD/test_dlghotspotcreate.h
