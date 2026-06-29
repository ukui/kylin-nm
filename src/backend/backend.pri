INCLUDEPATH += $$PWD
#include(hotspot/hotspot.pri)
include(dbus-interface/dbus-interface.pri)
include(../../common/common.pri)

HEADERS += \
    $$PWD/dbus.h \
    $$PWD/dbus_interface.h \
    $$PWD/proxyservicemanager.h \
    $$PWD/sysdbusregister.h \
#    $$PWD/utils.h


SOURCES += \
    $$PWD/dbus.cpp \
    $$PWD/dbus_interface.cpp \
    $$PWD/proxyservicemanager.cpp \
    $$PWD/sysdbusregister.cpp \
#    $$PWD/utils.cpp

