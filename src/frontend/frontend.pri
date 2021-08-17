INCLUDEPATH += $$PWD
include(tools/tools.pri)
include(wireless-security/wireless-security.pri)
include(xatom/xatom.pri)
include(tab-pages/tab-pages.pri)
include(list-items/list-items.pri)

FORMS += \
    $$PWD/confform.ui \
    $$PWD/mainwindow.ui \
    $$PWD/wificonfigdialog.ui \
    $$PWD/wpawifidialog.ui

HEADERS += \
    $$PWD/confform.h \
    $$PWD/customstyle.h \
    $$PWD/kylinwiredwidget.h \
    $$PWD/mainwindow.h \
    $$PWD/nmdemo.h \
    $$PWD/wificonfigdialog.h \
    $$PWD/wpawifidialog.h \
    $$PWD/new-mainwindow.h

SOURCES += \
    $$PWD/confform.cpp \
    $$PWD/customstyle.cpp \
    $$PWD/kylinwiredwidget.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/nmdemo.cpp \
    $$PWD/wificonfigdialog.cpp \
    $$PWD/wpawifidialog.cpp \
    $$PWD/new-mainwindow.cpp
