QT += widgets testlib network dbus
QT -= gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_lanitem

DESTDIR = .

INCLUDEPATH += ../../../../plugins/netconnect                 ../../../../pub                 ../../../../common                 /usr/include/ukcc/interface                 /usr/include/ukcc/widgets                 /usr/include/kysdk/applications                 /usr/include/x86_64-linux-gnu/qt5/QGSettings                 /usr/include/glib-2.0                 /usr/lib/x86_64-linux-gnu/glib-2.0/include

LIBS += -L../../../../plugins -lnetconnect         -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc -lgcov

PKGCONFIG += gsettings-qt              kysdk-qtwidgets              kysdk-sysinfo

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += test_lanitem.cpp            test_lanitem_main.cpp

HEADERS += test_lanitem.h

DEFINES += SRUN_QTEST
