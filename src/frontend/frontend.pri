INCLUDEPATH += $$PWD
include(tools/tools.pri)
include(wireless-security/wireless-security.pri)

FORMS += \
    $$PWD/confform.ui \
    $$PWD/mainwindow.ui \
    $$PWD/oneconnform.ui \
    $$PWD/onelancform.ui \
    $$PWD/wificonfigdialog.ui \
    $$PWD/wpawifidialog.ui

HEADERS += \
    $$PWD/confform.h \
    $$PWD/kylinwiredwidget.h \
    $$PWD/mainwindow.h \
    $$PWD/nmdemo.h \
    $$PWD/oneconnform.h \
    $$PWD/onelancform.h \
    $$PWD/wificonfigdialog.h \
    $$PWD/wpawifidialog.h

SOURCES += \
    $$PWD/confform.cpp \
    $$PWD/kylinwiredwidget.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/nmdemo.cpp \
    $$PWD/oneconnform.cpp \
    $$PWD/onelancform.cpp \
    $$PWD/wificonfigdialog.cpp \
    $$PWD/wpawifidialog.cpp
