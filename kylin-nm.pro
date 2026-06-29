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
        sys-dbus-netctrl \
        kylin-network-manager-enhance-daemon

QT += widgets
