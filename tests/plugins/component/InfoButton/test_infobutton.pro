QT += widgets testlib network dbus gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_infobutton

DESTDIR = .

INCLUDEPATH += ../../../../../plugins/component/InfoButton \
                ../../../../../plugins/netconnect \
                ../../../../../pub \
                ../../../../../common \
                /usr/include/ukcc/interface \
                /usr/include/ukcc/widgets \
                /usr/include/x86_64-linux-gnu/qt5/QGSettings \
                /usr/include/glib-2.0 \
                /usr/lib/x86_64-linux-gnu/glib-2.0/include

LIBS += -L../../../../../plugins -lnetconnect \
        -L/usr/lib/x86_64-linux-gnu -lkysdk-qtwidgets -lgsettings-qt -lukcc -lgcov

PKGCONFIG += gsettings-qt \
             kysdk-qtwidgets \
             kysdk-sysinfo

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += test_infobutton.cpp \
           main.cpp

HEADERS += test_infobutton.h

DEFINES += SRUN_QTEST
