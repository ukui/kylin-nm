QT += core gui widgets dbus testlib x11extras network KWindowSystem

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_joinhiddenwifipage

INCLUDEPATH += ../../../../../src/frontend
INCLUDEPATH += ../../../../../src/frontend/netdetails
INCLUDEPATH += ../../../../../src/frontend/tools
INCLUDEPATH += ../../../../../src/frontend/networkmode
INCLUDEPATH += ../../../../../src
INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/kysdk/applications

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_KEYWORDS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS
QMAKE_LFLAGS += -lKF5NetworkManagerQt
LIBS += -lKF5NetworkManagerQt

SOURCES += ../../../../../src/frontend/netdetails/joinhiddenwifipage.cpp
SOURCES += ../../../../../src/frontend/netdetails/securitypage.cpp
SOURCES += ../../../../../src/frontend/tools/divider.cpp
SOURCES += ../../../../../src/frontend/tools/kylable.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kylinutil.cpp
SOURCES += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp
SOURCES += test_joinhiddenwifipage.cpp
SOURCES += main.cpp

HEADERS += test_joinhiddenwifipage.h
HEADERS += ../../../../../src/frontend/netdetails/coninfo.h
HEADERS += ../../../../../src/frontend/netdetails/joinhiddenwifipage.h
HEADERS += ../../../../../src/frontend/netdetails/securitypage.h
HEADERS += ../../../../../src/frontend/tools/divider.h
HEADERS += ../../../../../src/frontend/tools/kylable.h
HEADERS += ../../../../../src/backend/dbus-interface/kywirelessconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectoperation.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinconnectsetting.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h
HEADERS += ../../../../../src/backend/dbus-interface/kylinutil.h
HEADERS += ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h
