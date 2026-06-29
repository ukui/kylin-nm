QT += core network testlib
QT -= gui

CONFIG += qt console testcase
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_qt-local-peer

# Source files - include the source under test
SOURCES +=     main.cpp     test_qt-local-peer.cpp     ../../../../src/singleapplication/qt-local-peer.cpp     ../../../../src/singleapplication/qt-locked-file.cpp     ../../../../src/singleapplication/qt-locked-file-unix.cpp

# Header files - need moc for Q_OBJECT
HEADERS +=     test_qt-local-peer.h     ../../../../src/singleapplication/qt-local-peer.h

# Include paths - point to the source directory
INCLUDEPATH += ../../../../src/singleapplication

# GCC coverage flags
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

# Google Test
CONFIG(debug, debug|release) {
    GTEST_DIR = /usr/src/gtest
    GMOCK_DIR = /usr/src/gmock
} else {
    GTEST_DIR = /usr/src/gtest
    GMOCK_DIR = /usr/src/gmock
}

INCLUDEPATH += $$GTEST_DIR $$GMOCK_DIR

# Link Google Test and Google Mock
LIBS += -L$$GTEST_DIR -lgtest -lgmock -lpthread

# Disable warnings for gtest
DEFINES += GTEST_HAS_STD_WSTRING=1
