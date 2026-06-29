QT += dbus testlib core network

CONFIG += qt console
CONFIG -= app_bundle
CONFIG += link_pkgconfig

TEMPLATE = app

TARGET = test_kylin_netctrol_server_interface

PKGCONFIG += glib-2.0 json-glib-1.0

LIBS += -lglib-2.0

INCLUDEPATH += ../../../../../sys-dbus-netctrl/kylin/common \
               ../../../../../sys-dbus-netctrl/kylin/netControl \
               ../../../../../sys-dbus-netctrl

SOURCES +=  test_kylin_netctrol_server_interface.cpp \
            main.cpp \
            ../../../../../sys-dbus-netctrl/kylin/common/kylin_cfgInfo.c \
            ../../../../../sys-dbus-netctrl/kylin/common/kylin_common.cpp \
            ../../../../../sys-dbus-netctrl/kylin/netControl/kylin_hedron_log.c \
            ../../../../../sys-dbus-netctrl/kylin/netControl/kylin_netctrol_server_interface.cpp \
            ../../../../../sys-dbus-netctrl/kylin/common/kylin_async_task.c

HEADERS += test_kylin_netctrol_server_interface.h \
           ../../../../../sys-dbus-netctrl/kylin/common/kylin_cfgInfo.h \
           ../../../../../sys-dbus-netctrl/kylin/common/kylin_common.h \
           ../../../../../sys-dbus-netctrl/kylin/netControl/kylin_hedron_log.h \
           ../../../../../sys-dbus-netctrl/kylin/netControl/kylin_netctrol_server_interface.h \
           ../../../../../sys-dbus-netctrl/kylin/common/kylin_async_task.h

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
