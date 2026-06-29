QT       += widgets testlib

QT       -= gui

TARGET = test_vpn
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    test_vpn.cpp

INCLUDEPATH +=  \
    ../../../plugins/vpn \
    ../../../plugins \
    ../../../common \
    ../../../component/AddBtn \
    /usr/include/ukcc/interface \
    /usr/include/ukcc/widgets \
    $$[QT_INSTALL_HEADERS] \
    /usr/include/kysdk/applications

LIBS += -L../../../../plugins -lvpn
LIBS += -L$$[QT_INSTALL_LIBS] -lukcc
LIBS += -lgcc_s

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -Wl,--allow-multiple-definition
QMAKE_CFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
