# Unit test project for proxyservicemanager

QT += core gui dbus network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

TARGET = test_proxyservicemanager

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords

PKGCONFIG += gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap

INCLUDEPATH += 205456PWD/../../../../src/backend
INCLUDEPATH += 205456PWD/../../../../pub
INCLUDEPATH += 205456PWD/../../../kt-test-utils/cpp-stub
INCLUDEPATH += 205456PWD/../../../kt-test-utils/cpp-stub-ext

LIBS += -L/usr/lib/ -lgsettings-qt -lX11 -lgcov
LIBS += -lgtest_main -lpthread -lgtest -lgmock

QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_CXXFLAGS += --coverage -fno-inline -fno-access-control

# Generate MOC files for Qt meta-object system
QT_MOC_RELAXED_META_RULES = yes

SOURCES += ../../../../src/backend/proxyservicemanager.cpp
SOURCES += test_proxyservicemanager.cpp

HEADERS += test_proxyservicemanager.h
HEADERS += ../../../../src/backend/proxyservicemanager.h
