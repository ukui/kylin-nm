TEMPLATE = subdirs

CONFIG += \
        ordered \
        qt \

SUBDIRS = \
        plugins/plugin.pro \
        src-vpn/src-vpn.pro \
        src \
        sys-dbus-register \
        kylin-nm-plugin/kylin-nm-plugin.pro \

QT += widgets
