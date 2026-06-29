QT += core gui widgets dbus network testlib

CONFIG += console c++14 qt

CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_kylinwiredconnectoperation

INCLUDEPATH += $$PWD/../../../../../../src
INCLUDEPATH += $$PWD/../../../../../../common

PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libsecret-1 gsettings-qt libcap

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += $$PKGCONFIG_CFLAGS
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
QMAKE_LFLAGS += -lgcov
QMAKE_LFLAGS += $$PKGCONFIG_LIBS

LIBS += -lgtest -lgtest_main -lpthread
LIBS += -lKF5NetworkManagerQt

SOURCES += $$PWD/test_kylinwiredconnectoperation.cpp \
           $$PWD/main.cpp \
           ../../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectoperation.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectsetting.cpp \
           ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.cpp \
           ../../../../../../src/backend/dbus-interface/kylinutil.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectitem.cpp \
           ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.cpp \
           ../../../../../../src/backend/dbus-interface/kylinconnectresource.cpp \
           ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.cpp

HEADERS += $$PWD/test_kylinwiredconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kylinwiredconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectoperation.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectsetting.h \
           ../../../../../../src/backend/dbus-interface/kylinnetworkresourcemanager.h \
           ../../../../../../src/backend/dbus-interface/kylinutil.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectitem.h \
           ../../../../../../src/backend/dbus-interface/kylinactiveconnectresource.h \
           ../../../../../../src/backend/dbus-interface/kylinconnectresource.h \
           ../../../../../../src/backend/dbus-interface/kylinnetworkdeviceresource.h
