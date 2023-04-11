INCLUDEPATH += $$PWD
include(hotspot/hotspot.pri)
include(dbus-interface/dbus-interface.pri)

HEADERS += \
    $$PWD/dbusadaptor.h \
    $$PWD/kylinarping.h \
    $$PWD/kylinipv4arping.h \
    $$PWD/kylinipv6arping.h \
    $$PWD/sysdbusregister.h \
    $$PWD/utils.h \
    $$PWD/wifi-auth-thread.h

SOURCES += \
    $$PWD/dbusadaptor.cpp \
    $$PWD/kylinipv4arping.cpp \
    $$PWD/kylinipv6arping.cpp \
    $$PWD/sysdbusregister.cpp \
    $$PWD/utils.cpp \
    $$PWD/wifi-auth-thread.cpp

DISTFILES +=
