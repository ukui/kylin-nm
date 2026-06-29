QT += widgets testlib
QT -= gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_connectdevlistitem

DESTDIR = .

INCLUDEPATH += ../../../../plugins/mobilehotspot                ../../../../pub                ../../../../common                /usr/include/kysdk/applications

LIBS += -L../../../../plugins -lmobilehotspot         -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc

PKGCONFIG += gsettings-qt              kysdk-qtwidgets              kysdk-sysinfo

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += test_connectdevlistitem.cpp

DEFINES += SRUN_QTEST
