QT += core dbus testlib gui qml quick

CONFIG += qt console warn_on depend_incremental link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_uisecurityconfig

DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += $$PWD/../../../../common
INCLUDEPATH += $$PWD/../../../../pub

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo

LIBS += -lpthread

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage -Wl,--no-as-needed

SOURCES += main.cpp
SOURCES += test_uisecurityconfig.cpp
SOURCES += mock_services.cpp
SOURCES += mock_functions.cpp
SOURCES += ../../../pub/uisecurityconfig.cpp

HEADERS += test_uisecurityconfig.h
HEADERS += ../../../pub/uisecurityconfig.h
HEADERS += mock_services.h
