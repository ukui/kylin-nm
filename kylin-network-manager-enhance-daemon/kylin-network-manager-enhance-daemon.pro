TARGET =nm-enhance-optimization
TEMPLATE = app
CONFIG += c++11  warn_on link_pkgconfig 
CONFIG -= app_bundle
CONFIG -= qt

PKGCONFIG +=dbus-1 glib-2.0 dbus-glib-1 gio-2.0 gio-unix-2.0

target.source += $$TARGET
target.path = /usr/bin


inst1.files += conf/com.kylin.network.enhancement.optimization.conf 
inst1.path = /etc/dbus-1/system.d/

inst2.files += conf/nm-enhance-optimization.service
inst2.path = /lib/systemd/system

inst3.files += conf/nm_enhance_dns.conf 
inst3.path = /etc/nm_enhance

inst4.files += conf/nm-enhance-optimization.service 
inst4.path = /usr/share/dbus-1/system-services

inst5.files += conf/kylin.hosts 
inst5.path = /etc/hosts.d/

inst6.files += conf/resolv.dnsmasq.conf 
inst6.path =/etc/

limit.files += conf/com.kylin.network.enhancement.optimization.limit 
limit.files += conf/com.kylin.network.enhancement.optimization.limit.verify
limit.path = /etc/dbus-1/conf/

limit1.files += conf/com.kylin.network.enhancement.optimization.limit 
limit1.files += conf/com.kylin.network.enhancement.optimization.limit.verify
limit1.path = /usr/share/dbus-1/conf/

limit2.files += conf/com.kylin.network.enhancement.optimization.yaml
limit2.path = /etc/kylin-config/basic/


INSTALLS += \
         target \
         inst1 \
         inst2 \
          inst3 \
         inst4 \
         inst5\
         inst6 \
         limit \
         limit1 \
         limit2 \


LIBS += -lpthread

INCLUDEPATH += $$PWD \
    $$PWD/kylin/common/ \
    $$PWD/kylin/netControl


SOURCES += \
        main.c \
	file_monitor.c \
	dbus-server.c 


HEADERS += \
	public.h \
	file_monitor.h \
	dns-xml.h \
	dbus-server.h 

