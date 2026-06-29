INCLUDEPATH += $$PWD
include(../../src/backend/dbus-interface/dbus-interface.pri)
include(../../common/common.pri)

HEADERS += \
    $$PWD/vpndbusadaptor.h

SOURCES += \
    $$PWD/vpndbusadaptor.cpp

DISTFILES +=
