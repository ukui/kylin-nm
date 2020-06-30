#-------------------------------------------------
#
# Project created by QtCreator 2018-10-19T15:29:47
#
#-------------------------------------------------

QT       += core gui x11extras dbus KWindowSystem

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kylin-nm
TEMPLATE = app
LANGUAGE = C++

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release

# CONFIG += link_pkgconfig
# PKGCONFIG += gsettings-qt
LIBS    +=  -L/usr/lib/ -lgsettings-qt

target.path = /usr/bin
target.source += $$TARGET
desktop.path = /etc/xdg/autostart/
desktop.files = kylin-nm.desktop

INSTALLS += target \
    desktop

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
    src/ksimplenm.cpp \
    src/kylin-dbus-interface.cpp \
    src/kylin-network-interface.c \
    src/loadingdiv.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/oneconnform.cpp \
    src/onelancform.cpp \
    src/utils.cpp \
    wireless-security/dlgconnhidwifi.cpp \
    wireless-security/dlgconnhidwifisecfast.cpp \
    wireless-security/dlgconnhidwifisectunneltls.cpp \
    wireless-security/dlgconnhidwifisecpeap.cpp \
    wireless-security/dlgconnhidwifisectls.cpp \
    wireless-security/dlgconnhidwifisecleap.cpp \
    wireless-security/dlgconnhidwifisecpwd.cpp \
    wireless-security/dlgconnhidwifiwep.cpp \
    wireless-security/dlgconnhidwifileap.cpp \
    wireless-security/dlgconnhidwifiwpa.cpp \
    wireless-security/kylinheadfile.cpp \
    hot-spot/dlghotspotcreate.cpp


HEADERS += \
    src/backthread.h \
    src/confform.h \
    src/ksimplenm.h \
    src/kylin-dbus-interface.h \
    src/kylin-network-interface.h \
    src/loadingdiv.h \
    src/mainwindow.h \
    src/oneconnform.h \
    src/onelancform.h \
    src/utils.h \
    wireless-security/dlgconnhidwifi.h \
    wireless-security/dlgconnhidwifisecfast.h \
    wireless-security/dlgconnhidwifisectunneltls.h \
    wireless-security/dlgconnhidwifisecpeap.h \
    wireless-security/dlgconnhidwifisectls.h \
    wireless-security/dlgconnhidwifisecleap.h \
    wireless-security/dlgconnhidwifisecpwd.h \
    wireless-security/dlgconnhidwifiwep.h \
    wireless-security/dlgconnhidwifileap.h \
    wireless-security/dlgconnhidwifiwpa.h \
    wireless-security/kylinheadfile.h \
    hot-spot/dlghotspotcreate.h

FORMS += \
    src/confform.ui \
    src/mainwindow.ui \
    src/oneconnform.ui \
    src/onelancform.ui \
    wireless-security/dlgconnhidwifi.ui \
    wireless-security/dlgconnhidwifisecfast.ui \
    wireless-security/dlgconnhidwifisectunneltls.ui \
    wireless-security/dlgconnhidwifisecpeap.ui \
    wireless-security/dlgconnhidwifisectls.ui \
    wireless-security/dlgconnhidwifisecleap.ui \
    wireless-security/dlgconnhidwifisecpwd.ui \
    wireless-security/dlgconnhidwifiwep.ui \
    wireless-security/dlgconnhidwifileap.ui \
    wireless-security/dlgconnhidwifiwpa.ui \
    hot-spot/dlghotspotcreate.ui

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

DISTFILES +=
