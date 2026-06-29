QT += testlib core network
QT -= gui

CONFIG += qt console warn_on thread
CONFIG -= app_bundle
CONFIG += link_pkgconfig

TEMPLATE = app

TARGET = test_kylin_cfgInfo

INCLUDEPATH += . ../../../../sys-dbus-netctrl/kylin/common ../../../../sys-dbus-netctrl/kylin ../../../../sys-dbus-netctrl ../../../../common

DEPENDPATH += . ../../../../sys-dbus-netctrl/kylin/common ../../../../sys-dbus-netctrl/kylin ../../../../sys-dbus-netctrl ../../../../common

PKGCONFIG += glib-2.0 json-glib-1.0

LIBS += -lgtest -lgtest_main -lpthread -lglib-2.0

SOURCES += test_kylin_cfgInfo.cpp main.cpp ../../../../sys-dbus-netctrl/kylin/common/kylin_cfgInfo.c ../../../../sys-dbus-netctrl/kylin/common/kylin_common.cpp

HEADERS += test_kylin_cfgInfo.h ../../../../sys-dbus-netctrl/kylin/common/kylin_cfgInfo.h ../../../../sys-dbus-netctrl/kylin/common/kylin_common.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_NO_DEBUG

QMAKE_CXXFLAGS += --coverage
QMAKE_CFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
