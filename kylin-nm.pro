TEMPLATE = subdirs

CONFIG += ordered \
    qt

SUBDIRS = \
        src \

TRANSLATIONS += \
	translations/kylin-nm_zh_CN.ts \
        translations/kylin-nm_tr.ts \
	translations/kylin-nm_bo.ts

QT += widgets
