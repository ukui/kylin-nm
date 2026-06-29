QT += widgets testlib dbus
QT -= gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_mobilehotspotwidget

DESTDIR = .

INCLUDEPATH += ../../../../plugins/mobilehotspot \
                ../../../../pub \
                ../../../../common \
                /usr/include/kysdk/applications \
                /usr/include/ukcc/interface

LIBS += -L/home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/plugins -lmobilehotspot \
        -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             kysdk-sysinfo

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage

SOURCES += test_mobilehotspotwidget.cpp

DEFINES += SRUN_QTEST
