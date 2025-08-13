#-------------------------------------------------
#
# Project created by QtCreator 2019-08-22T11:12:59
#
#-------------------------------------------------
QT += core gui x11extras dbus KWindowSystem svg concurrent network
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent sql xml
TEMPLATE = app

TARGET = unit_test_alltest
target.source += $$TARGET
target.path = ./

#代码覆盖率工具gcov
QMAKE_LFLAGS +=-fprofile-arcs -ftest-coverage
QMAKE_CXXFLAGS += --coverage -fno-inline -fno-access-control -fno-exceptions

LIBS          += -lgtest_main -lpthread
LIBS          +=  -L$$[QT_INSTALL_LIBS] -lgsettings-qt  -lgtest -lgcov
# -ldconf -lfontconfig -lfreetype

CONFIG += c++14 qt warn_on link_pkgconfig no_keywords
#CONFIG += release
PKGCONFIG +=gio-2.0 glib-2.0 gio-unix-2.0 libnm libnma libsecret-1 gtk+-3.0 gsettings-qt libcap kysdk-qtwidgets kysdk-waylandhelper
PKGCONFIG +=kysdk-sysinfo

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

LIBS    +=  -L/usr/lib/ -lgsettings-qt -lX11 -lKF5NetworkManagerQt -lukui-log4qt -lgcov -lgtest_main -lpthread
#INCLUDEPATH += /usr/include/dconf

#打桩工具
TEST_UTILS_PATH = $$PWD/../kt-test-utils

# Include directories
INCLUDEPATH += $$PROJECT_ROOTDIR \
               $$TEST_UTILS_PATH/cpp-stub \
               $$TEST_UTILS_PATH/cpp-stub-ext \

SOURCES += unit_test_kywirelessconnectoperation.cpp \
unit_test_wifi-auth-thread.cpp \
unit_test_kywirelessnetitem.cpp \
unit_test_kylinutil.cpp \
unit_test_kylinnetworkdeviceresource.cpp \
unit_test_kyenterpricesettinginfo.cpp \
unit_test_kylinapconnectitem.cpp \
unit_test_kylinbluetoothconnectitem.cpp \
unit_test_kylinconnectitem.cpp \
unit_test_kylinvpnconnectitem.cpp \
unit_test_kyvpnconnectoperation.cpp \
unit_test_kylinactiveconnectresource.cpp \
unit_test_kylinnetworkresourcemanager.cpp \
unit_test_kylinconnectoperation.cpp \
unit_test_kylinconnectresource.cpp \
           main.cpp

include(../../src/backend/dbus-interface/dbus-interface.pri)
HEADERS += ../../src/backend/dbus-interface/kywirelessconnectoperation.h \
../../src/backend/wifi-auth-thread.h \
../../src/backend/dbus-interface/kywirelessnetitem.h \
../../src/backend/dbus-interface/kylinutil.h \
../../src/backend/dbus-interface/kylinnetworkdeviceresource.h \
../../src/backend/dbus-interface/kylinapconnectitem.h \
../../src/backend/dbus-interface/kylinbluetoothconnectitem.h \
../../src/backend/dbus-interface/kyenterpricesettinginfo.h \
../../src/backend/dbus-interface/kylinconnectitem.h \
../../src/backend/dbus-interface/kylinvpnconnectitem.h \
../../src/backend/dbus-interface/kylinactiveconnectresource.h \
../../src/backend/dbus-interface/kylinnetworkresourcemanager.h \
../../src/backend/dbus-interface/kyvpnconnectoperation.h \
../../src/backend/dbus-interface/kylinconnectoperation.h \
../../src/backend/dbus-interface/kylinconnectresource.h \
