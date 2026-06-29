QT += core gui widgets testlib

CONFIG += qt console warn_on
CONFIG -= app_bundle

TARGET = test_switchbutton

SOURCES += ./test_switchbutton.cpp

HEADERS += ./test_switchbutton.h

INCLUDEPATH += ../../../../../../plugins/component/SwitchButton

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
