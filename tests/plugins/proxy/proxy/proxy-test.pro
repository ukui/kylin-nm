QT += widgets testlib dbus
QT -= gui

CONFIG += qt console c++11 link_pkgconfig
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_proxy

DESTDIR = .

INCLUDEPATH += ../../../../plugins/proxy \
                ../../../../pub \
                ../../../../common \
                /usr/include/ukcc/interface \
                /usr/include/ukcc/widgets \
                /usr/include/x86_64-linux-gnu/qt5/QGSettings \
                /usr/include/glib-2.0 \
                /usr/lib/x86_64-linux-gnu/glib-2.0/include

HEADERS += test_proxy.h

SOURCES += test_proxy.cpp main.cpp

LIBS += -L../../../../plugins -lproxy \
        -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc

PKGCONFIG += gsettings-qt kysdk-qtwidgets kysdk-sysinfo gio-2.0 gio-unix-2.0
