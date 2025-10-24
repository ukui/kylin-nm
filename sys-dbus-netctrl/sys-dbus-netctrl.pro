QT += dbus network
TARGET = kylin-nm-netctrl
TEMPLATE = app
CONFIG += c++14 qt warn_on link_pkgconfig no_keywords
CONFIG -= app_bundle

TARGET = kylin-nm-netctrl
TEMPLATE = app

PKGCONFIG +=glib-2.0 json-glib-1.0

target.source += $$TARGET
target.path = /usr/bin

inst1.files += conf/com.kylin.networkCtrol.service
inst1.path = /usr/share/dbus-1/system-services/
inst2.files += conf/com.kylin.networkCtrol.conf
inst2.path = /etc/dbus-1/system.d/
inst3.files += conf/com.kylin.networkCtrol.yaml
inst3.path = /etc/kylin-config/basic/
#服务集管控 配置.limit、.limit.verify文件，放入/etc/dbus-1/conf/目录 和 /usr/share/dbus-1/conf/目录
limit.files += conf/com.kylin.networkCtrol.limit
limit.files += conf/com.kylin.networkCtrol.limit.verify
limit.path = /etc/dbus-1/conf/
limit2.files += conf/com.kylin.networkCtrol.limit
limit2.files += conf/com.kylin.networkCtrol.limit.verify
limit2.path = /usr/share/dbus-1/conf/


INSTALLS += \
         target \
         inst1 \
         inst2 \
         limit \
         limit2 \
         inst3 \

INCLUDEPATH += $$PWD \
    $$PWD/kylin/common/ \
    $$PWD/kylin/netControl


SOURCES += \
        main.cpp \
	$$PWD/kylin/common/kylin_cfgInfo.c \
	$$PWD/kylin/common/kylin_common.cpp \
	$$PWD/kylin/netControl/kylin_hedron_log.c \
	$$PWD/kylin/netControl/kylin_netctrol_server_interface.cpp \
        $$PWD/kylin/common/kylin_async_task.c

HEADERS += \
	$$PWD/kylin/common/kylin_cfgInfo.h \
	$$PWD/kylin/common/kylin_common.h \
	$$PWD/kylin/netControl/kylin_hedron_log.h \
	$$PWD/kylin/netControl/kylin_netctrol_server_interface.h \ 
        $$PWD/kylin/common/kylin_async_task.h
