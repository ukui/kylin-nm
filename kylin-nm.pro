TEMPLATE = subdirs

CONFIG += ordered \
    qt

SUBDIRS = \
#    plugins/plugin.pro \
        src \

TRANSLATIONS += \
        translations/kylin-nm_zh_CN.ts \
        translations/kylin-nm_tr.ts \
        translations/kylin-nm_bo.ts
