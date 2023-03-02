QT       += widgets network dbus gui core KWindowSystem
TEMPLATE = lib
CONFIG += plugin

include(../component/infobutton.pri)
include(../component/divider.pri)
include(../component/deviceframe.pri)
include(../component/RadioItemButton.pri)
include(../component/fixlabel.pri)
include(../component/listitemedit.pri)
include(../component/pages.pri)
include(../component/kylinarping.pri)
include(hiddenwifi/hiddenwifi.pri)

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
        translations/wlan_zh_CN.ts \
        translations/wlan_tr.ts \
        translations/wlan_bo.ts \
        translations/wlan_bo_CN.ts

RESOURCES += \
    resource.qrc

DISTFILES += \
    translations/wlan_bo_CN.ts
