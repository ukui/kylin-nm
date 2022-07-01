#QT -= gui
QT       += core gui x11extras dbus KWindowSystem svg concurrent network

TARGET = kylin-nm-external
TEMPLATE = lib
DEFINES += LIBNMEXTERNALKYLIN_LIBRARY

CONFIG += c++11 qt warn_on link_pkgconfig no_keywords

PKGCONFIG +=libnm libnma libsecret-1 gsettings-qt libcap

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += $$PWD/../libnm-base-kylin/

LIBS +=  -L/usr/lib/ -lgsettings-qt -lX11 -lKF5NetworkManagerQt
LIBS += -L$$PWD/../libnm-base-kylin/ -l kylin-nm-base

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(kylin-agent/kylin-agent.pri)
include(kylin-tool/kylin-tool.pri)
#include(kylin-dbus/kylin-dbus.pri)

SOURCES +=

HEADERS += \
    libnm-external-kylin_global.h

DESTDIR += $$PWD
# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
