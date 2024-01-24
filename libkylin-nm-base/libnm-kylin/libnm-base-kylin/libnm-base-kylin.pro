#QT -= gui
#QT       += core gui x11extras dbus KWindowSystem svg concurrent network
QT       += core dbus network

TARGET = kylin-nm-base
TEMPLATE = lib
DEFINES += LIBNMBASEKYLIN_LIBRARY

CONFIG += c++11 qt warn_on link_pkgconfig no_keywords
#CONFIG += create_pc create_prl no_install_prl


PKGCONFIG +=libnm gsettings-qt gtk+-3.0
#PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libnma libsecret-1  gsettings-qt
#INCLUDEPATH += /usr/include/dbus-1.0/dbu

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

SOURCES += \
    depend/kyapnetresource.cpp \
    depend/kyenterpricesettinginfo.cpp \
    depend/kylinactiveconnectresource.cpp \
    depend/kylinconnectoperation.cpp \
    depend/kylinconnectsetting.cpp \
    depend/kylingeneral.cpp \
    depend/kylinnetresource.cpp \
    depend/kylinnetworkdeviceresource.cpp \
    depend/kylinnetworkmanagerutil.cpp \
    kylinnetworkmanger.cpp \
    depend/kylinnetworkresourcemanager.cpp \
    depend/kylinwiredconnectoperation.cpp \
    depend/kylinwirednetresource.cpp \
    depend/kywirelessconnectoperation.cpp \
    depend/kywirelessnetresource.cpp
    kylinnetworkmanger.cpp \

HEADERS += \
    libnm-base-kylin_global.h \
    kylin-nm-interface.h \
    kylinnetworkmanager.h \
    depend/kyapnetresource.h \
    depend/kyenterpricesettinginfo.h \
    depend/kylinactiveconnectresource.h \
    depend/kylinapconnectitem.h \
    depend/kylinconnectitem.h \
    depend/kylinconnectoperation.h \
    depend/kylinconnectsetting.h \
    depend/kylingeneral.h \
    depend/kylinnetresource.h \
    depend/kylinnetworkdeviceresource.h \
    depend/kylinnetworkmanagerutil.h \
    depend/kylinnetworkresourcemanager.h \
    depend/kylinnetworkresourcetype.h \
    depend/kylinwiredconnectoperation.h \
    depend/kylinwirednetresource.h \
    depend/kywirelessconnectoperation.h \
    depend/kywirelessnetitem.h \
    depend/kywirelessnetresource.h

DESTDIR += $$PWD
# Default rules for deployment.
unix {
    target.path = /usr/lib/

    manager.path = /usr/include/kylin-nm
    manager.files = kylinnetworkmanager.h \
                    libnm-base-kylin_global.h \
                    kylin-nm-interface.h

    depend.path =   /usr/include/kylin-nm/depend
    depend.files =  depend/*.h

#    gschema.files = org.ukui.kylin-nm.switch.gschema.xml
#    gschema.path = /usr/share/glib-2.0/schemas/


    INSTALLS += manager \
                depend \
                gschema

#    QMAKE_PKGCONFIG_NAME = kylin-nm-base
#    QMAKE_PKGCONFIG_DESCRIPTION = kylin-nm Header files
#    QMAKE_PKGCONFIG_VERSION = $$VERSION
#    QMAKE_PKGCONFIG_LIBDIR = $$target.path
#    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
#    QMAKE_PKGCONFIG_INCDIR = $$INCLUDEPATH
##/usr/include/kylin-nm /usr/include/KF5/NetworkManagerQt
##    QMAKE_PKGCONFIG_INCDIR += /usr/include/KF5/NetworkManagerQt
#    QMAKE_PKGCONFIG_CFLAGS += -I/usr/include/kylin-nm
#    QMAKE_PKGCONFIG_REQUIRES = gio-2.0 glib-2.0 libnm
#    QMAKE_INSTALL_DIR = /usr/share/pkgconfig/
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    org.ukui.kylin-nm.switch.gschema.xml
