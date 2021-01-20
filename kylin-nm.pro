#-------------------------------------------------
#
# Project created by QtCreator 2018-10-19T15:29:47
#
#-------------------------------------------------

QT       += core gui x11extras dbus KWindowSystem svg concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kylin-nm
TEMPLATE = app
LANGUAGE = C++

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release

# CONFIG += link_pkgconfig
# PKGCONFIG += gsettings-qt
LIBS    +=  -L/usr/lib/ -lgsettings-qt -lX11

target.path = /usr/bin
target.source += $$TARGET
desktop.path = /etc/xdg/autostart/
desktop.files = kylin-nm.desktop

inst1.files += src/conf/com.kylin.NetworkManager.qt.systemdbus.service
inst1.path = /usr/share/dbus-1/system-services/
inst2.files += src/conf/com.kylin.NetworkManager.qt.systemdbus.conf
inst2.path = /etc/dbus-1/system.d/

INSTALLS += target \
    desktop \
    inst1 \
    inst2 \

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)


SOURCES += \
    src/backthread.cpp \
    src/confform.cpp \
    src/dbusadaptor.cpp \
    src/ksimplenm.cpp \
    src/kylin-dbus-interface.cpp \
    src/kylin-network-interface.c \
    src/loadingdiv.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/oneconnform.cpp \
    src/onelancform.cpp \
    src/switchbutton.cpp \
    src/sysdbusregister.cpp \
    src/utils.cpp \
    src/wpawifidialog.cpp \
    wireless-security/dlghidewifi.cpp \
    wireless-security/dlghidewifieapfast.cpp \
    wireless-security/dlghidewifieapleap.cpp \
    wireless-security/dlghidewifieappeap.cpp \
    wireless-security/dlghidewifieappwd.cpp \
    wireless-security/dlghidewifieaptls.cpp \
    wireless-security/dlghidewifieapttls.cpp \
    wireless-security/dlghidewifileap.cpp \
    wireless-security/dlghidewifiwep.cpp \
    wireless-security/dlghidewifiwpa.cpp \
    wireless-security/kylinheadfile.cpp \
    hot-spot/dlghotspotcreate.cpp


HEADERS += \
    src/backthread.h \
    src/confform.h \
    src/dbusadaptor.h \
    src/ksimplenm.h \
    src/kylin-dbus-interface.h \
    src/kylin-network-interface.h \
    src/loadingdiv.h \
    src/mainwindow.h \
    src/oneconnform.h \
    src/onelancform.h \
    src/switchbutton.h \
    src/sysdbusregister.h \
    src/utils.h \
    src/wpawifidialog.h \
    wireless-security/dlghidewifi.h \
    wireless-security/dlghidewifieapfast.h \
    wireless-security/dlghidewifieapleap.h \
    wireless-security/dlghidewifieappeap.h \
    wireless-security/dlghidewifieappwd.h \
    wireless-security/dlghidewifieaptls.h \
    wireless-security/dlghidewifieapttls.h \
    wireless-security/dlghidewifileap.h \
    wireless-security/dlghidewifiwep.h \
    wireless-security/dlghidewifiwpa.h \
    wireless-security/kylinheadfile.h \
    hot-spot/dlghotspotcreate.h

FORMS += \
    src/confform.ui \
    src/mainwindow.ui \
    src/oneconnform.ui \
    src/onelancform.ui \
    hot-spot/dlghotspotcreate.ui \
    src/wpawifidialog.ui \
    wireless-security/dlghidewifi.ui \
    wireless-security/dlghidewifieapfast.ui \
    wireless-security/dlghidewifieapleap.ui \
    wireless-security/dlghidewifieappeap.ui \
    wireless-security/dlghidewifieappwd.ui \
    wireless-security/dlghidewifieaptls.ui \
    wireless-security/dlghidewifieapttls.ui \
    wireless-security/dlghidewifileap.ui \
    wireless-security/dlghidewifiwep.ui \
    wireless-security/dlghidewifiwpa.ui

RESOURCES += \
    nmqrc.qrc

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

TRANSLATIONS = translations/kylin-nm_zh_CN.ts \
               translations/kylin-nm_tr.ts \
               translations/kylin-nm_bo.ts

DISTFILES += \
    src/conf/com.kylin.NetworkManager.qt.systemdbus.conf \
    src/conf/com.kylin.NetworkManager.qt.systemdbus.service
