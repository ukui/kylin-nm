QT += core dbus testlib  qml quick

CONFIG += qt console warn_on depend_incremental link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_knmwlandatakeeper

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/../../../../qml-network/plugin
INCLUDEPATH += $$PWD/../../../../qml-network
INCLUDEPATH += $$PWD/../../../../common
INCLUDEPATH += $$PWD/../../../../pub

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper kysdk-sysinfo
LIBS += -lpthread

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage -Wl,--no-as-needed

SOURCES += main.cpp
SOURCES += test_knmwlandatakeeper.cpp
SOURCES += ../../../../qml-network/plugin/networkqml_plugin.cpp
SOURCES += ../../../../qml-network/plugin/knminterface.cpp
SOURCES += ../../../../qml-network/plugin/knmdbuscaller.cpp
SOURCES += ../../../../qml-network/plugin/netdevice.cpp
SOURCES += ../../../../qml-network/plugin/wirelessconnectionmodel.cpp
SOURCES += ../../../../qml-network/plugin/knmdatakeeper.cpp
SOURCES += ../../../../qml-network/plugin/knmlandatakeeper.cpp
SOURCES += ../../../../qml-network/plugin/knmwlandatakeeper.cpp
SOURCES += ../../../../pub/uisecurityconfig.cpp

HEADERS += test_knmwlandatakeeper.h
HEADERS += ../../../../qml-network/plugin/networkqml_plugin.h
HEADERS += ../../../../qml-network/plugin/knminterface.h
HEADERS += ../../../../qml-network/plugin/knmdbuscaller.h
HEADERS += ../../../../qml-network/plugin/netdevice.h
HEADERS += ../../../../qml-network/plugin/wirelessconnectionmodel.h
HEADERS += ../../../../qml-network/plugin/knmdatakeeper.h
HEADERS += ../../../../qml-network/plugin/knmlandatakeeper.h
HEADERS += ../../../../qml-network/plugin/knmwlandatakeeper.h
HEADERS += ../../../../pub/uisecurityconfig.h
