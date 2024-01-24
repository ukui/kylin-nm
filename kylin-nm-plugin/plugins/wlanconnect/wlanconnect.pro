QT       += widgets network dbus gui core
TEMPLATE = lib
CONFIG += plugin

TARGET = $$qtLibraryTarget(wlanconnect)
DESTDIR = ../..
target.path = /usr/lib/kylin-nm

INCLUDEPATH   +=  \
                 $$PROJECT_COMPONENTSOURCE \
                 $$PROJECT_ROOTDIR \

LIBS          +=  -L$$[QT_INSTALL_LIBS]  -lkylin-nm-base -lkylin-keyring

CONFIG += c++11 link_pkgconfig no_keywords qt warn_on no_install_prl ordered
#CONFIG += create_pc create_prl


PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
#             kylin-nm-base \
             libnm \
             gtk+-3.0 \
             libsecret-1

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
LIBS    +=  -L/usr/lib/ -lKF5NetworkManagerQt

INCLUDEPATH += $$PWD/../../../libkylin-nm-base/libnm-kylin/libnm-base-kylin
LIBS += -L$$PWD/../../../libkylin-nm-base/libnm-kylin/libnm-base-kylin/ -lkylin-nm-base

INCLUDEPATH += $$PWD/../../../libkylin-nm-base/kylin-keyring
LIBS += -L$$PWD/../../../libkylin-nm-base/kylin-keyring/ -lkylin-keyring

#DEFINES += QT_DEPRECATED_WARNINGS

include(../component/infobutton.pri)
include(../component/divider.pri)
include(../component/deviceframe.pri)
include(../component/RadioItemButton.pri)
include(../component/fixlabel.pri)
include(hiddenwifi/hiddenwifi.pri)

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
