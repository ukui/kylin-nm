QT       += widgets network dbus gui core
TEMPLATE = lib
CONFIG += plugin

include(../component/infobutton.pri)
include(../component/divider.pri)
include(../component/deviceframe.pri)
include(../component/RadioItemButton.pri)
include(../component/fixlabel.pri)

TARGET = $$qtLibraryTarget(netconnect)
DESTDIR = ../..
target.path = /usr/lib/kylin-nm

INCLUDEPATH   +=  \
                 $$PROJECT_COMPONENTSOURCE \
                 $$PROJECT_ROOTDIR \

LIBS          +=  -L$$[QT_INSTALL_LIBS] -lkylin-nm-base

CONFIG += c++11 \
          link_pkgconfig \
          no_keywords

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             kylin-nm-base

#DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    itemframe.cpp \
    lanitem.cpp \
    netconnect.cpp

HEADERS += \
    itemframe.h \
    lanitem.h \
    netconnect.h

INSTALLS += target \

TRANSLATIONS += \
        translations/zh_CN.ts \
        translations/tr.ts \
        translations/bo.ts
