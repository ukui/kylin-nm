QT += widgets core gui testlib
CONFIG += qt console testcase
CONFIG -= app_bundle

TEMPLATE = app

TARGET = test_itemframe

INCLUDEPATH += ../../../../plugins/wlanconnect                ../../../../common                ../../../../pub                ../../../../plugins/component/DrownLabel                ../../../../plugins/component/AddBtn                /usr/include/ukcc/interface                /usr/include/ukcc/widgets                /usr/include/x86_64-linux-gnu/qt5/QGSettings                /usr/include/kysdk/applications

LIBS += -L../../../../plugins -lwlanconnect         -L/usr/lib/x86_64-linux-gnu -lgsettings-qt -lukcc

PKGCONFIG += gsettings-qt kysdk-qtwidgets kysdk-sysinfo

SOURCES += test_itemframe.cpp

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
LIBS += -lgcov
