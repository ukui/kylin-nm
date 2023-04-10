TEMPLATE = subdirs

CONFIG += \
        ordered \
        qt

SUBDIRS = \
        plugins/plugin.pro \
        src-vpn/src-vpn.pro \
        src

TRANSLATIONS += \
        translations/kylin-nm_zh_CN.ts \
        translations/kylin-nm_tr.ts \
        translations/kylin-nm_bo.ts \
        translations/kylin-nm_bo_CN.ts

CONFIG(release, debug|release) {
    !system($$PWD/translate_generation.sh): error("Failed to generate translation")
}

qm_files.path = $${PREFIX}/share/kylin-nm/kylin-nm/
qm_files.files = translations/*.qm

INSTALLS += qm_files \
