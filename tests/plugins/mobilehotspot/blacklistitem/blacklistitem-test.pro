QT += widgets testlib
QT -= gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_blacklistitem

DESTDIR = .

INCLUDEPATH += ../../../../plugins/mobilehotspot \
                ../../../../pub \
                ../../../../common \
                /usr/include/kysdk/applications

LIBS += /home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/plugins/libmobilehotspot.so \
        -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             kysdk-sysinfo

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage -lgcov

SOURCES += test_blacklistitem.cpp

DEFINES += SRUN_QTEST
