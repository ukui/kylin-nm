TEMPLATE = subdirs


CONFIG += \
        ordered \
        qt \
        qt

SUBDIRS = \
        plugins/plugin.pro \
        src-vpn/src-vpn.pro \
        src

QT += widgets
