INCLUDEPATH += $$PWD
include(tools/tools.pri)
include(xatom/xatom.pri)
include(tab-pages/tab-pages.pri)
include(list-items/list-items.pri)
include(netdetails/netdetails.pri)
include(enterprise-wlan/enterprise-wlan.pri)

FORMS += \
    $$PWD/wificonfigdialog.ui

HEADERS += \
    $$PWD/customstyle.h \
    $$PWD/kylinwiredwidget.h \
    $$PWD/mainwindow.h \
    $$PWD/nmdemo.h \
    $$PWD/wificonfigdialog.h

SOURCES += \
    $$PWD/customstyle.cpp \
    $$PWD/kylinwiredwidget.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/nmdemo.cpp \
    $$PWD/wificonfigdialog.cpp
