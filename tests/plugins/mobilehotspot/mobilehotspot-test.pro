QT += widgets testlib
QT -= gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_mobilehotspot

DESTDIR = .

INCLUDEPATH += ../../../../plugins/mobilehotspot \
                ../../../../pub \
                ../../../../common \
                /usr/include/ukcc/interface \
                /usr/include/ukcc/widgets \
                /usr/include/kysdk/applications

LIBS += -L../../../../plugins -lmobilehotspot \
        -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             kysdk-sysinfo

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += test_mobilehotspot.cpp \
           main.cpp

HEADERS += test_mobilehotspot.h

DEFINES += SRUN_QTEST
