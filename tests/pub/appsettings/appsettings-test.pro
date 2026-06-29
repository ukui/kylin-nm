QT += core testlib

CONFIG += qt console warn_on depend_incremental
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_appsettings

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../../../pub

SOURCES += main.cpp
SOURCES += test_appsettings.cpp
SOURCES += ../../../pub/appsettings.cpp
HEADERS += test_appsettings.h
HEADERS += ../../../pub/appsettings.h

# 覆盖率配置
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage -Wl,--no-as-needed
