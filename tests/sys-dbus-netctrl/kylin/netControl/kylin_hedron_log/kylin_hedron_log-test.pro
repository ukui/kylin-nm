QT += testlib core network
QT -= gui

CONFIG += qt console warn_on thread
CONFIG -= app_bundle
CONFIG += link_pkgconfig

TEMPLATE = app

TARGET = test_kylin_hedron_log

INCLUDEPATH += . ../../../../../sys-dbus-netctrl/kylin/common ../../../../../sys-dbus-netctrl/kylin/netControl ../../../../../sys-dbus-netctrl

DEPENDPATH += . ../../../../../sys-dbus-netctrl/kylin/common ../../../../../sys-dbus-netctrl/kylin/netControl ../../../../../sys-dbus-netctrl

PKGCONFIG += glib-2.0 json-glib-1.0

LIBS += -lgtest -lgtest_main -lpthread -lgmock

SOURCES += test_kylin_hedron_log.cpp main.cpp ../../../../../sys-dbus-netctrl/kylin/common/kylin_common.cpp ../../../../../sys-dbus-netctrl/kylin/netControl/kylin_hedron_log.c

HEADERS += test_kylin_hedron_log.h ../../../../../sys-dbus-netctrl/kylin/common/kylin_common.h ../../../../../sys-dbus-netctrl/kylin/netControl/kylin_hedron_log.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_NO_DEBUG

QMAKE_CXXFLAGS += --coverage
QMAKE_CFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
