QT += core

CONFIG += console c++14 qt
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_qt-locked-file

INCLUDEPATH += ../../../src
INCLUDEPATH += ../../../src/singleapplication

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov

SOURCES +=     ../../../src/singleapplication/qt-locked-file.cpp     ../../../src/singleapplication/qt-locked-file-unix.cpp     test_qt-locked-file.cpp     main.cpp

HEADERS +=     test_qt-locked-file.h
