QT       += core
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

TARGET = test_qt-locked-file

INCLUDEPATH += ../../../../src-vpn/singleapplication
INCLUDEPATH += /usr/include/gtest
INCLUDEPATH += /usr/include/gmock

LIBS += -lgtest -lgtest_main -lgmock -lpthread

QMAKE_CXXFLAGS += --coverage
QMAKE_CXXFLAGS += -include qt-locked-file-wrapper.h
QMAKE_LFLAGS += -lgcov --coverage

DEFINES += QT_NO_DEBUG

SOURCES +=     ../../../../src-vpn/singleapplication/qt-locked-file.cpp     ../../../../src-vpn/singleapplication/qt-locked-file-unix.cpp     test_qt-locked-file.cpp     main.cpp

HEADERS +=     ../../../../src-vpn/singleapplication/qt-locked-file.h     qt-locked-file-wrapper.h
