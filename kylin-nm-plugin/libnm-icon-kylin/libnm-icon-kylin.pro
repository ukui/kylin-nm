QT += core gui widgets dbus network svg

TEMPLATE = lib
DEFINES += LIBNMICONKYLIN_LIBRARY

CONFIG += c++14 qt link_pkgconfig no_keywords

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

PKGCONFIG += gio-2.0 libnm

INCLUDEPATH += /usr/include/KF5/NetworkManagerQt
LIBS    +=  -L/usr/lib/ -lKF5NetworkManagerQt

#PKGCONFIG += kylin-nm-base
INCLUDEPATH += $$PWD/../../libkylin-nm-base/libnm-kylin/libnm-base-kylin
LIBS += -L$$PWD/../../libkylin-nm-base/libnm-kylin/libnm-base-kylin/ -lkylin-nm-base

#INCLUDEPATH += $$PWD/../libnm-base-kylin/
#INCLUDEPATH += /usr/include/KF5/NetworkManagerQt

#LIBS += -L$$PWD/../libnm-base-kylin/ -l kylin-nm-base

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kynetworkicon.cpp \
    imageutil.cpp

HEADERS += \
    kynetworkicon.h \
    imageutil.h

# Default rules for deployment.
unix {
    target.path = /usr/lib

    manager.path = /usr/include/kylin-nm
    manager.files = kynetworkicon.h

    INSTALLS += manager
}
!isEmpty(target.path): INSTALLS += target
