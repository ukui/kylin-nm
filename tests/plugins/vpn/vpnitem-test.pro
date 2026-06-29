QT       += widgets testlib

QT       -= gui

TARGET = test_vpnitem
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    test_vpnitem.cpp

INCLUDEPATH +=  \
    ../../../../plugins/vpn \
    ../../../../plugins \
    ../../../../common \
    ../../../../component/AddBtn \
    /usr/include/ukcc/interface \
    /usr/include/ukcc/widgets \
    /usr/include/x86_64-linux-gnu/qt5/QGSettings \
    /usr/include/kysdk/applications

LIBS += -L../../../../plugins -lvpn
LIBS += -L/usr/lib/x86_64-linux-gnu -lukcc -lgcc_s 

QMAKE_CXXFLAGS += -fno-profile-arcs -fno-test-coverage

QMAKE_LFLAGS += -Wl,--allow-multiple-definition
