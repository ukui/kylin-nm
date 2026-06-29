QT += widgets testlib dbus
QT -= gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_blacklistpage

DESTDIR = .

INCLUDEPATH += ../../../../plugins/mobilehotspot \
                ../../../../pub \
                ../../../../common \
                /usr/include/ukcc/interface \
                /usr/include/ukcc/widgets \
                /usr/include/kysdk/applications

LIBS += -L/home/ll-v11/桌面/dd/kylin-nm/v11-2603/unit-tests/kylin-nm/plugins -lmobilehotspot \
        -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             kysdk-sysinfo

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov -lgcov

SOURCES += test_blacklistpage.cpp \
           main.cpp

HEADERS += test_blacklistpage.h

DEFINES += SRUN_QTEST

