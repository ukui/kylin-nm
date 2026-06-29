QT += core gui dbus x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_kylinapconnectitem
TEMPLATE = app

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 dbus-1 libnm

INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common
#INCLUDEPATH += /usr/include/dbus-1.0
#INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include
#INCLUDEPATH += /usr/include/glib-2.0
#INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lgcov
LIBS += -ldbus-1

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += main.cpp \
           test_kylinapconnectitem.cpp \
           ../../../../../src/backend/dbus-interface/kylinapconnectitem.cpp

HEADERS += test_kylinapconnectitem.h \
           ../../../../../src/backend/dbus-interface/kylinapconnectitem.h

