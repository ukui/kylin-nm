INCLUDEPATH += $$PWD
include(hotspot/hotspot.pri)
include(dbus-interface/dbus-interface.pri)

HEADERS += \
    $$PWD/backthread.h \
    $$PWD/dbusadaptor.h \
    $$PWD/ksimplenm.h \
    $$PWD/kylin-dbus-interface.h \
    $$PWD/kylin-network-interface.h \
    $$PWD/kylinipv4arping.h \
    $$PWD/sysdbusregister.h \
    $$PWD/utils.h \
    $$PWD/wifi-auth-thread.h

SOURCES += \
    $$PWD/backthread.cpp \
    $$PWD/dbusadaptor.cpp \
    $$PWD/ksimplenm.cpp \
    $$PWD/kylin-dbus-interface.cpp \
    $$PWD/kylin-network-interface.c \
    $$PWD/kylinipv4arping.cpp \
    $$PWD/sysdbusregister.cpp \
    $$PWD/utils.cpp \
    $$PWD/wifi-auth-thread.cpp

DISTFILES +=
