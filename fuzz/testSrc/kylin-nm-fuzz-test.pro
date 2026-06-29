QT       += core dbus network gui widgets

# Disable AFL sanitizer flags to avoid missing header errors
DEFINES += AFL_DISABLE_TRADITIONAL

# Disable Qt signals/slots keywords to avoid conflict with glib headers
DEFINES += QT_NO_KEYWORDS

# AFL compiler flags
QMAKE_CFLAGS += -D_GNU_SOURCE
QMAKE_CXXFLAGS += -D_GNU_SOURCE

TARGET = kylin-nm-fuzz-test
TEMPLATE = app
CONFIG += console c++11 warn_on link_pkgconfig
CONFIG -= app_bundle

DEFINES += FUZZ_TEST

INCLUDEPATH += ../../pub

SOURCES += fuzz_test_main.cpp
SOURCES += fuzz_appsettings_fuzz.cpp
SOURCES += ../../pub/appsettings.cpp

HEADERS += ../../pub/appsettings.h
HEADERS += sanitizer/lsan_interface.h