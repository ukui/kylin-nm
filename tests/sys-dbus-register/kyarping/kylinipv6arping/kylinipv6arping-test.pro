QT += testlib dbus widgets core network
QT -= gui

CONFIG += qt console warn_on thread
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kylinipv6arping


INCLUDEPATH += .  \
               ../../../../sys-dbus-register  \
               ../../../../sys-dbus-register/kyarping \
               ../../../../common \
               /usr/include/KF5/NetworkManagerQt \
               /usr/include/libnm \
               /usr/include/glib-2.0 \
               /usr/lib/x86_64-linux-gnu/glib-2.0/include

DEPENDPATH += . \
              ../../../../sys-dbus-register  \
              ../../../../sys-dbus-register/kyarping \
              ../../../../common

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libcap

LIBS += -L/usr/lib/ -lKF5NetworkManagerQt -lcap

SOURCES += test_kylinipv6arping.cpp \
           main.cpp \
           ../../../../sys-dbus-register/kyarping/kylinipv6arping.cpp

HEADERS += test_kylinipv6arping.h \
           ../../../../sys-dbus-register/kyarping/kylinipv6arping.h \
           ../../../../sys-dbus-register/kyarping/kylinarping.h \
           ../../../../sys-dbus-register/authority-base.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_NO_DEBUG

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
