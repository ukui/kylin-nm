QT += widgets testlib

CONFIG += qt console c++11
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_aptproxydialog

INCLUDEPATH += ../../../../plugins/proxy \
                ../../../../pub \
                ../../../../common \
                /usr/include/ukcc/interface \
                /usr/include/ukcc/widgets

HEADERS += test_aptproxydialog.h

SOURCES += test_aptproxydialog.cpp main.cpp

LIBS += -L../../../../plugins -lproxy \
        -L/usr/lib -lgsettings-qt -lukcc

PKGCONFIG += gio-2.0 gio-unix-2.0 gsettings-qt kysdk-qtwidgets kysdk-sysinfo
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
