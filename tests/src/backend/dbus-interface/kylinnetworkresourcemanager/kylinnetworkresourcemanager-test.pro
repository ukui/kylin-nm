QT += core gui dbus network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_kylinnetworkresourcemanager
TEMPLATE = app

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 dbus-1 libnm

INCLUDEPATH += ../../../../../src/backend/dbus-interface
INCLUDEPATH += ../../../../../common
INCLUDEPATH += /usr/include/dbus-1.0
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lgcov
LIBS += -ldbus-1
LIBS += -lKF5NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES += main.cpp \
test_kylinnetworkresourcemanager.cpp \
../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp \
../../../../../src/backend/dbus-interface/kylinutil.cpp

HEADERS += test_kylinnetworkresourcemanager.h \
../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h \
../../../../../src/backend/dbus-interface/kylinutil.h

