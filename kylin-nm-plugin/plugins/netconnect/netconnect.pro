QT       += widgets network dbus gui core
TEMPLATE = lib
CONFIG += plugin

TARGET = $$qtLibraryTarget(netconnect)
DESTDIR = ../..
target.path = /usr/lib/kylin-nm

INCLUDEPATH   +=  \
                 $$PROJECT_COMPONENTSOURCE \
                 $$PROJECT_ROOTDIR \

CONFIG += c++11 link_pkgconfig no_keywords qt warn_on no_install_prl ordered
#CONFIG += create_pc create_prl

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             libnm \
             gtk+-3.0
#             kylin-nm-base

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
LIBS    +=  -L/usr/lib/ -lKF5NetworkManagerQt

INCLUDEPATH += $$PWD/../../../libkylin-nm-base/libnm-kylin/libnm-base-kylin
LIBS += -L$$PWD/../../../libkylin-nm-base/libnm-kylin/libnm-base-kylin/ -lkylin-nm-base
#DEFINES += QT_DEPRECATED_WARNINGS

include(../component/infobutton.pri)
include(../component/divider.pri)
include(../component/deviceframe.pri)
include(../component/RadioItemButton.pri)
include(../component/fixlabel.pri)

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
        translations/bo.ts \
        translations/bo_CN.ts

RESOURCES += \
    resource.qrc

DISTFILES += \
    translations/bo_CN.ts
