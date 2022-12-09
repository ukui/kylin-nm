
#LIBINTERFACE_NAME = $$qtLibraryTarget(kylinarping)

SOURCES += \
        $$PWD/KylinArping/kylinipv4arping.cpp \
        $$PWD/KylinArping/kylinipv6arping.cpp \

HEADERS += \
        $$PWD/KylinArping/kylinarping.h \
        $$PWD/KylinArping/kylinipv4arping.h \
        $$PWD/KylinArping/kylinipv6arping.h \
