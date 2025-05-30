TEMPLATE = subdirs


CONFIG += \
        ordered \
        qt \

SUBDIRS = \
        plugins/plugin.pro \
        src-vpn/src-vpn.pro \
        src \
        sys-dbus-register \
        qml-network \

QT += widgets
