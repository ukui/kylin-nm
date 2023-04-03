INCLUDEPATH += $$PWD
#include(tools/tools.pri)
#include(xatom/xatom.pri)
#include(tab-pages/tab-pages.pri)
#include(list-items/list-items.pri)
#include(netdetails/netdetails.pri)
#include(enterprise-wlan/enterprise-wlan.pri)
#include(single-pages/single-pages.pri)
include(networkmode/networkmode.pri)

FORMS += \
#    $$PWD/wificonfigdialog.ui

HEADERS += \
    $$PWD/customstyle.h \
    $$PWD/mainwindow.h \
#    $$PWD/wificonfigdialog.h \
#    $$PWD/vpnobject.h

SOURCES += \
    $$PWD/customstyle.cpp \
    $$PWD/mainwindow.cpp \
#    $$PWD/wificonfigdialog.cpp \
#    $$PWD/vpnobject.cpp
