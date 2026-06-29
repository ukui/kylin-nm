QT += testlib core network
QT -= gui

CONFIG += qt console warn_on thread
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kylinipv4arping

INCLUDEPATH += .                 ../../../../sys-dbus-register                 ../../../../sys-dbus-register/kyarping                 ../../../../common

DEPENDPATH += .               ../../../../sys-dbus-register               ../../../../sys-dbus-register/kyarping               ../../../../common

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libcap

LIBS += -lcap

SOURCES += test_kylinipv4arping.cpp  \
           main.cpp \
           ../../../../sys-dbus-register/kyarping/kylinipv4arping.cpp

HEADERS += test_kylinipv4arping.h \
           ../../../../sys-dbus-register/kyarping/kylinipv4arping.h \
           ../../../../sys-dbus-register/kyarping/kylinarping.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_NO_DEBUG

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
