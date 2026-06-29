QT += widgets testlib network dbus gui

CONFIG += qt console warn_on
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_deviceframe

DESTDIR = .

INCLUDEPATH += ../../../../plugins/wlanconnect                 ../../../../pub                 ../../../../common                 ../../../../plugins/component/DrownLabel                 /usr/include/ukcc/interface                 /usr/include/ukcc/widgets                 /usr/include/kysdk/applications                 /usr/include/x86_64-linux-gnu/qt5/QGSettings                 /usr/include/glib-2.0                 /usr/lib/x86_64-linux-gnu/glib-2.0/include

LIBS += -L../../../../plugins -lwlanconnect         -L/usr/lib/x86_64-linux-gnu -lkysdk-qtwidgets -lgsettings-qt -lukcc

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage -lgcov -Wl,-rpath,"../../../../plugins"

PKGCONFIG += gsettings-qt              kysdk-qtwidgets              kysdk-sysinfo

SOURCES += test_deviceframe.cpp            main.cpp

HEADERS += test_deviceframe.h

DEFINES += SRUN_QTEST
