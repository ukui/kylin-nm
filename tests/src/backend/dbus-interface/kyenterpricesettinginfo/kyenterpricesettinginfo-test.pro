QT += core gui network dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_kyenterpricesettinginfo
TEMPLATE = app

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 dbus-1 libnm libsecret-1

INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
INCLUDEPATH += /usr/include/dbus-1.0
#INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include
INCLUDEPATH += /usr/include/glib-2.0
#INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += $$[QT_INSTALL_LIBS]/glib-2.0/include
INCLUDEPATH += $$[QT_INSTALL_LIBS]/dbus-1.0/include

LIBS += -lKF5NetworkManagerQt
LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lgcov
LIBS += -ldbus-1

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_CFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += main.cpp test_kyenterpricesettinginfo.cpp ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.cpp

HEADERS += test_kyenterpricesettinginfo.h ../../../../../src/backend/dbus-interface/kyenterpricesettinginfo.h
