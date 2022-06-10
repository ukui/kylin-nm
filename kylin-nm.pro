TEMPLATE = subdirs

CONFIG += ordered \
    qt

SUBDIRS = \
    plugins/plugin.pro \
    libnm-kylin \
#    kylin-nm-gui \
#    kylin-nm-server \

#kylin-nm-gui.depends = libnm-kylin
#kylin-nm-server.depends = libnm-kylin

#TRANSLATIONS += \
#        translations/kylin-nm_zh_CN.ts \
#        translations/kylin-nm_tr.ts \
#        translations/kylin-nm_bo.ts
