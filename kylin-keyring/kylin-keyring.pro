QT       += core gui x11extras dbus KWindowSystem svg concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kylin-keyring
TEMPLATE = lib
DEFINES += LIBKYLIN_KEYRING_LIBRARY

CONFIG += c++14 qt warn_on link_pkgconfig console
CONFIG += no_keywords

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libnma libsecret-1 gtk+-3.0 gsettings-qt libcap kysdk-qtwidgets

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

LIBS    +=  -L/usr/lib/ -lKF5NetworkManagerQt

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

#include(kylin-agent/kylin-agent.pri)
INCLUDEPATH += $$PWD
INCLUDEPATH += ./kylin-agent

HEADERS += \
    ./kylin-agent/agentutils.h \
    ./kylin-agent/kylinagent.h \
#    ./kylin-agent/kylinagentobject.h \
    ./kylin-agent/kylinpassworddialog.h \
    ./kylin-agent/kylinsecretagent.h

SOURCES += \
    ./kylin-agent/kylinagent.cpp \
#    ./kylin-agent/kylinagentobject.cpp \
    ./kylin-agent/kylinpassworddialog.cpp \
    ./kylin-agent/kylinsecretagent.cpp \

TRANSLATIONS += \
    ./translations/kylin-keyring_zh_CN.ts \
    ./translations/kylin-keyring_tr.ts \
    ./translations/kylin-keyring_bo.ts

unix {
    target.path = /usr/lib/

    manager.path = /usr/include/kylin-nm/kylin-agent
    manager.files = kylin-agent/agentutils.h \
                    kylin-agent/kylinagent.h \
                    kylin-agent/kylinagentobject.h \
                    kylin-agent/kylinpassworddialog.h \
                    kylin-agent/kylinsecretagent.h

    INSTALLS += manager
}
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    svg.qrc
