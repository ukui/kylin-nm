TEMPLATE = subdirs

CONFIG += \
        ordered \
        qt \

SUBDIRS = \
        libkylin-nm-base/libkylin-nm-base.pro \
        plugins/plugin.pro \
        src-vpn/src-vpn.pro \
        src \
        sys-dbus-register \
        kylin-nm-plugin/kylin-nm-plugin.pro

QT += widgets
