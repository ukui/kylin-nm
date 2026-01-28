QT += dbus network
TARGET = kylin-nm-sysdbus
TEMPLATE = app
CONFIG += c++14 qt warn_on link_pkgconfig no_keywords
CONFIG -= app_bundle

TARGET = kylin-nm-sysdbus
TEMPLATE = app

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libcap

inst1.files += conf/com.kylin.network.qt.systemdbus.service
inst1.path = /usr/share/dbus-1/system-services/
inst2.files += conf/com.kylin.network.qt.systemdbus.conf
inst2.path = /usr/share/dbus-1/system.d/
inst3.files += conf/kylin-nm-sysdbus.service
inst3.path = /usr/lib/systemd/system/
target.source += $$TARGET
target.path = /usr/bin

INCLUDEPATH += /usr/include/KF6/NetworkManagerQt /usr/include/libnm

LIBS    +=  -L/usr/lib/  -lKF6NetworkManagerQt

INSTALLS += \
         target \
         inst1 \
         inst2 \
         inst3

include(kyarping/kyarping.pri)

SOURCES += \
        kynmsystemdbus.cpp \
        main.cpp

HEADERS += \
        authority-base.h \
        kynmsystemdbus.h
