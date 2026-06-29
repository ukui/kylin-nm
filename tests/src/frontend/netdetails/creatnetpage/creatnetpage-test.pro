QT += core gui widgets dbus testlib x11extras network KWindowSystem

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_creatnetpage

INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../common
INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications
INCLUDEPATH += /usr/include/kysdk/kysdk-system
INCLUDEPATH += /usr/include/kysdk/kysdk-base

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS
QMAKE_LFLAGS += -lKF5NetworkManagerQt

LIBS += -lKF5NetworkManagerQt
LIBS += -L/lib -lkylin-nm-base

SOURCES += ../../../../../src/frontend/netdetails/creatnetpage.cpp
SOURCES += ../../../../../src/frontend/netdetails/multiplednswidget.cpp
SOURCES += ../../../../../src/frontend/netdetails/dnssettingwidget.cpp
SOURCES += ../../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../../src/frontend/tools/listitemedit.cpp
SOURCES += ../../../../../src/frontend/tools/divider.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += test_creatnetpage.cpp
SOURCES += main.cpp

HEADERS += test_creatnetpage.h
HEADERS += ../../../../../src/frontend/netdetails/creatnetpage.h
HEADERS += ../../../../../src/frontend/netdetails/multiplednswidget.h
HEADERS += ../../../../../src/frontend/netdetails/dnssettingwidget.h
HEADERS += ../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../src/frontend/tools/listitemedit.h
HEADERS += ../../../../../src/frontend/tools/divider.h
HEADERS += ../../../../../src/frontend/netdetails/coninfo.h
