QT += core gui widgets dbus testlib
CONFIG += console c++14 qt
CONFIG -= app_bundle
TEMPLATE = app
TARGET = test_sysdbusregister
INCLUDEPATH += ../../../../src
INCLUDEPATH += ../../../../src/backend
INCLUDEPATH += ../../../../common
INCLUDEPATH += ../../../../pub
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QGSettings
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += /usr/include/libnm
INCLUDEPATH += /usr/include/kysdk/applications
INCLUDEPATH += /usr/include/kysdk/kysdk-system
INCLUDEPATH += /usr/include/kysdk/sysinfo
INCLUDEPATH += /usr/include/kysdk/ukui
INCLUDEPATH += /usr/include/libsecret-1
INCLUDEPATH += /usr/include/KF5/KWindowSystem
INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
HEADERS += \
    ../../../../src/backend/sysdbusregister.h \
    test_sysdbusregister.h
SOURCES += \
    ../../../../src/backend/sysdbusregister.cpp \
    test_sysdbusregister.cpp \
    main.cpp
