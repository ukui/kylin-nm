QT += testlib core network dbus
QT -= gui

CONFIG += qt console warn_on thread
CONFIG -= app_bundle
CONFIG += link_pkgconfig

TEMPLATE = app

TARGET = test_kylin_common

INCLUDEPATH +=  . \
                ../../../../sys-dbus-netctrl/kylin/common \
                ../../../../sys-dbus-netctrl/kylin \
                ../../../../sys-dbus-netctrl \
                ../../../../common

DEPENDPATH +=   . \
                ../../../../sys-dbus-netctrl/kylin/common \
                ../../../../sys-dbus-netctrl/kylin \
                ../../../../sys-dbus-netctrl \
                ../../../../common

PKGCONFIG += glib-2.0 json-glib-1.0

LIBS += -lgtest -lgtest_main -lpthread -lgmock

SOURCES += test_kylin_common.cpp \
main.cpp \
../../../../sys-dbus-netctrl/kylin/common/kylin_common.cpp \
../../../../sys-dbus-netctrl/kylin/common/kylin_cfgInfo.c  \
../../../../sys-dbus-netctrl/kylin/common/kylin_async_task.c

HEADERS += ../../../../sys-dbus-netctrl/kylin/common/kylin_common.h \
../../../../sys-dbus-netctrl/kylin/common/kylin_cfgInfo.h \
../../../../sys-dbus-netctrl/kylin/common/kylin_async_task.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_NO_DEBUG

QMAKE_CXXFLAGS += --coverage
QMAKE_CFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
