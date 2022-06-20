QT       += widgets network dbus gui core
TEMPLATE = lib
CONFIG += plugin

include(../component/infobutton.pri)
include(../component/divider.pri)
include(../component/deviceframe.pri)
include(../component/RadioItemButton.pri)

TARGET = $$qtLibraryTarget(wlanconnect)
DESTDIR = ../..
target.path = /usr/lib/kylin-nm

INCLUDEPATH   +=  \
                 $$PROJECT_COMPONENTSOURCE \
                 $$PROJECT_ROOTDIR \

LIBS          +=  -L$$[QT_INSTALL_LIBS]  -lkylin-nm-base -lkylin-keyring

CONFIG += c++11 \
          link_pkgconfig \
          no_keywords

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             kylin-nm-base \
             libsecret-1

#DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    itemframe.cpp \
    wlanconnect.cpp \
    wlanitem.cpp

HEADERS += \
    itemframe.h \
    wlanconnect.h \
    wlanitem.h

INSTALLS += target \

TRANSLATIONS += \
        translations/zh_CN.ts \
        translations/tr.ts \
        translations/bo.ts
