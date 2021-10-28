QT       += widgets network dbus gui core
TEMPLATE = lib
CONFIG += plugin

include(../component/switchbutton.pri)
include(../component/infobutton.pri)

TARGET = $$qtLibraryTarget(netconnect)
DESTDIR = ../..
target.path = $$[QT_INSTALL_LIBS]/ukui-control-center
trans.files = translations/*
trans.path = /usr/share/kylin-nm/netconnect/

INCLUDEPATH   +=  \
                 $$PROJECT_COMPONENTSOURCE \
                 $$PROJECT_ROOTDIR \
                 /usr/include/ukcc/interface \
                 /usr/include/ukcc/widgets

LIBS          +=  -L$$[QT_INSTALL_LIBS] -lgsettings-qt -lukcc

CONFIG += c++11 \
          link_pkgconfig \

PKGCONFIG += gsettings-qt \

#DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    addnetbtn.cpp \
    deviceframe.cpp \
    drownlabel.cpp \
    itemframe.cpp \
    lanitem.cpp \
    netconnect.cpp

HEADERS += \
    addnetbtn.h \
    deviceframe.h \
    drownlabel.h \
    itemframe.h \
    lanitem.h \
    netconnect.h

FORMS += \
    netconnect.ui

INSTALLS += target \
            trans

TRANSLATIONS += \
        translations/zh_CN.ts \
        translations/tr.ts \
        translations/bo.ts
