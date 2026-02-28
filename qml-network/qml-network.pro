TEMPLATE = lib
TARGET = ukui-shortcut-network
QT += quick dbus core
CONFIG += plugin c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS

PKGCONFIG += gsettings-qt6

LIBS += -lpthread
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
        plugin/CSingleton.h \
        plugin/knmdatakeeper.h \
        plugin/knmdbuscaller.h \
        plugin/knminterface.h \
        plugin/knmlandatakeeper.h \
        plugin/knmwlandatakeeper.h \
        plugin/netdevice.h \
        plugin/networkqml_plugin.h \
        plugin/wirelessconnectionmodel.h

SOURCES += \
        plugin/knmdatakeeper.cpp \
        plugin/knmdbuscaller.cpp \
        plugin/knminterface.cpp \
        plugin/knmlandatakeeper.cpp \
        plugin/knmwlandatakeeper.cpp \
        plugin/netdevice.cpp \
        plugin/networkqml_plugin.cpp \
        plugin/wirelessconnectionmodel.cpp

RESOURCES += widget/qml.qrc

CONFIG(release, debug|release) {
    !system($$PWD/translate_generation.sh): error("Failed to generate translation")
}

inst1.files += widget/**
inst1.path = /usr/share/ukui/widgets/org.ukui.shortcut.network

inst2.files += plugin/qmldir
inst2.path = $$[QT_INSTALL_LIBS]/qt6/qml/org/ukui/shortcut/network

qm_files.files += translations/*.qm
qm_files.path = /usr/share/ukui/widgets/org.ukui.shortcut.network/translations

ts_files.files += translations/*.ts
ts_files.path = /usr/share/ukui/widgets/org.ukui.shortcut.network/translations


target.path = $$[QT_INSTALL_LIBS]/qt6/qml/org/ukui/shortcut/network

TRANSLATIONS += \
    translations/ukui-shortcut-network_zh_CN.ts \
    translations/ukui-shortcut-network_zh_Hant.ts \
    translations/ukui-shortcut-network_zh_HK.ts \
    translations/ukui-shortcut-network_ug.ts \
    translations/ukui-shortcut-network_mn.ts \
    translations/ukui-shortcut-network_ky.ts \
    translations/ukui-shortcut-network_kk.ts \
    translations/ukui-shortcut-network_bo_CN.ts


INSTALLS += inst1 \
            inst2 \
            qm_files \
            ts_files \
            target

OBJECTS_DIR = ./obj/
MOC_DIR = ./moc/

DISTFILES += \
    widget/ui/LanTop.qml \
    widget/ui/NetScrollViewArea.qml \
    widget/ui/WlanTop.qml
