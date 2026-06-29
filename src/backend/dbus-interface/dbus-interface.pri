INCLUDEPATH += $$PWD

QT += concurrent
include(../vpnsecrets/vpnsecrets.pri)

HEADERS += \
    $$PWD/agentutils.h \
    $$PWD/gsystem-local-alloc.h \
    $$PWD/kyenterpricesettinginfo.h \
    $$PWD/kylinactiveconnectresource.h \
    $$PWD/kylinagent.h \
    $$PWD/kylinapconnectitem.h \
#    $$PWD/kylinbluetoothconnectitem.h \
    $$PWD/kylinconnectitem.h \
    $$PWD/kylinconnectoperation.h \
    $$PWD/kylinconnectresource.h \
    $$PWD/kylinconnectsetting.h \
    $$PWD/kylinnetworkdeviceresource.h \
    $$PWD/kylinnetworkresourcemanager.h \
    $$PWD/kylinsecretagent.h \
    $$PWD/kylinutil.h \
    $$PWD/kylinvpnconnectitem.h \
   # $$PWD/kylinvpnrequest.h \
    $$PWD/kylinwiredconnectoperation.h \
    $$PWD/kywirelessconnectoperation.h \
    $$PWD/kywirelessnetitem.h \
    $$PWD/kywirelessnetresource.h \
    $$PWD/kyvpnconnectoperation.h \
    $$PWD/nm-macros-internal.h

SOURCES += \
    $$PWD/kyenterpricesettinginfo.cpp \
    $$PWD/kylinactiveconnectresource.cpp \
    $$PWD/kylinagent.cpp \
    $$PWD/kylinapconnectitem.cpp \
#    $$PWD/kylinbluetoothconnectitem.cpp \
    $$PWD/kylinconnectitem.cpp \
    $$PWD/kylinconnectoperation.cpp \
    $$PWD/kylinconnectresource.cpp \
    $$PWD/kylinconnectsetting.cpp \
    $$PWD/kylinnetworkdeviceresource.cpp \
    $$PWD/kylinnetworkresourcemanager.cpp \
    $$PWD/kylinsecretagent.cpp \
    $$PWD/kylinutil.cpp \
    $$PWD/kylinvpnconnectitem.cpp \
    #$$PWD/kylinvpnrequest.c \
    $$PWD/kylinwiredconnectoperation.cpp \
    $$PWD/kywirelessconnectoperation.cpp \
    $$PWD/kywirelessnetitem.cpp \
    $$PWD/kyvpnconnectoperation.cpp \
    $$PWD/kywirelessnetresource.cpp

